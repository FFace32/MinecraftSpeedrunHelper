#include "hkOpenGL.h"
#include "../Drawing.h"
#include "../Math/AABB.h"
#include "../Math/Math.h"
#include "../SDK/net/minecraft/client/Minecraft.h"
#include "../SDK/net/minecraft/client/player/LocalPlayer.h"
#include "../SDK/net/minecraft/world/level/Level.h"
#include "../SDK/net/minecraft/world/entity/EntityType.h"
#include "../SDK/net/minecraft/world/item/ItemStack.h"
#include "../SDK/net/minecraft/world/InteractionHand.h"
#include "../SDK/net/minecraft/world/item/BowItem.h"
#include "../SDK/net/minecraft/world/item/Items.h"
#include "../SDK/net/minecraft/core/BlockPos.h"
#include "../SDK/net/minecraft/world/level/block/state/BlockState.h"
#include "../SDK/net/minecraft/world/phys/shapes/VoxelShape.h"
#include "../SDK/net/minecraft/world/level/ClipContext.h"
#include "../SDK/net/minecraft/world/phys/shapes/EntityCollisionContext.h"
#include "../SDK/net/minecraft/world/phys/BlockHitResult.h"
#include "../SDK/net/minecraft/world/phys/HitResult.h"
#include "../SDK/net/minecraft/world/entity/projectile/ProjectileUtil.h"
#include "../SDK/net/minecraft/world/phys/EntityHitResult.h"
#include "../SDK/net/minecraft/world/level/LevelReader.h"
#include "../SDK/net/minecraft/tags/FluidTags.h"
#include "../SDK/net/minecraft/world/level/material/FluidState.h"
#include "../Config.h"

using namespace Minecraft;

void ESP( const Drawing::Context& Context, net::minecraft::client::player::LocalPlayer* LocalPlayer, net::minecraft::world::level::Level* Level )
{
	// Look for entities 64 blocks in all directions around the player.
	const auto Entities = Level->getEntities( LocalPlayer, Math::AABB( LocalPlayer->position() ).Inflate( 64.0 ).ToMinecraft(), nullptr );
	for ( const auto& Entity : *Entities )
	{
		auto Type = Entity->getType();
		if ( g_Java.GetEnv()->IsSameObject( Type, net::minecraft::world::entity::EntityType::END_CRYSTAL() ) )
			DrawBox( Context, Entity->getBoundingBox(), Drawing::Color( 255, 0, 0 ) );
	}
}

enum class HitType
{
	None,
	Entity,
	Block
};
struct BowTrajectoryPoint
{
	BowTrajectoryPoint( const Math::Vector& Location, HitType Hit ) :
		Location( Location ),
		Hit( Hit )
	{ }

	Math::Vector Location;
	HitType Hit;
};
using BowTrajectory = std::vector<BowTrajectoryPoint>;

BowTrajectory GetBowTrajectory( net::minecraft::client::player::LocalPlayer* LocalPlayer, net::minecraft::world::level::Level* Level )
{
	BowTrajectory Trajectory;
	
	auto Bow = LocalPlayer->getItemInHand( net::minecraft::world::InteractionHand::MAIN_HAND() )->getItem().CastTo<net::minecraft::world::item::BowItem*>();
	if ( net::minecraft::world::item::Item::getId( Bow ) != net::minecraft::world::item::Items::BOW )
		Bow = LocalPlayer->getItemInHand( net::minecraft::world::InteractionHand::OFF_HAND() )->getItem().CastTo<net::minecraft::world::item::BowItem*>();

	if ( net::minecraft::world::item::Item::getId( Bow ) != net::minecraft::world::item::Items::BOW )
		return Trajectory;

	// Reversed releaseUsing
	const auto UseItemRemainingTicks = LocalPlayer->getUseItemRemainingTicks();
	if ( UseItemRemainingTicks <= 0 )
		return Trajectory;

	const auto Power = net::minecraft::world::item::BowItem::getPowerForTime( Bow->getUseDuration() - UseItemRemainingTicks );
	if ( Power < 0.01f )
		return Trajectory;

	const auto LocalPlayerPosition = LocalPlayer->position();
	auto Position = Math::Vector( LocalPlayerPosition->x(), LocalPlayer->getEyeY() - 0.10000000149011612, LocalPlayerPosition->z() );

	Trajectory.emplace_back( Position, HitType::None );
	
	auto xRot = LocalPlayer->xRot();
	auto yRot = LocalPlayer->yRot();

	// Reversed shootFromRotation and shoot
	const auto cxRot = Math::cosd( xRot );
	auto DeltaMovement = Math::Vector( -Math::sind( yRot ) * static_cast<double>( cxRot ), static_cast<double>( -Math::sind( xRot ) ), Math::cosd( yRot ) * static_cast<double>( cxRot ) ).Normalize();
	// Spread would get added here (random * 0.007499999832361937)
	DeltaMovement *= Power * 3.0;

	yRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.x, DeltaMovement.z ) ) );
	xRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.y, DeltaMovement.GetHorizontalDistance() ) ) );
	
	/*auto LocalPlayerDeltaMovement = LocalPlayer->getDeltaMovement();
	DeltaMovement.x += LocalPlayerDeltaMovement->x();
	DeltaMovement.y += LocalPlayer->isOnGround() ? 0.0 : LocalPlayerDeltaMovement->y();
	DeltaMovement.z += LocalPlayerDeltaMovement->z();*/

	// Reversed tick and baseTick
	auto inGround = false;
	auto removed = false;
	while ( !inGround && !removed )
	{
		auto xRotO = xRot;
		auto yRotO = yRot;

		if ( xRotO == 0.f && yRotO == 0.f )
		{
			yRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.x, DeltaMovement.z ) ) );
			xRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.y, DeltaMovement.GetHorizontalDistance() ) ) );
			yRotO = yRot;
			xRotO = xRot;
		}

		const auto Vec3Position = Position.ToMinecraft();
		const auto BlockPosition = net::minecraft::core::BlockPos::init( static_cast<int>( floor( Position.x ) ), static_cast<int>( floor( Position.y ) ), static_cast<int>( floor( Position.z ) ) );

		const auto BlockState = Level->getBlockState( BlockPosition );
		if ( !BlockState->isAir() )
		{
			const auto CollisionShape = BlockState->getCollisionShape( Level, BlockPosition );
			if ( !CollisionShape->isEmpty() )
			{
				const auto AABBs = CollisionShape->toAabbs();
				for ( const auto& AABB : *AABBs )
				{
					if ( AABB->move( BlockPosition )->contains( Vec3Position ) )
					{
						inGround = true;
						break;
					}
				}
			}
		}

		if ( inGround )
			return Trajectory;

		auto Vec3NextPosition = ( Position + DeltaMovement ).ToMinecraft();
		const auto ClipContext = net::minecraft::world::level::ClipContext::init( Vec3Position, Vec3NextPosition, net::minecraft::world::level::ClipContext::Block::COLLIDER(),
																			net::minecraft::world::level::ClipContext::Fluid::NONE(), LocalPlayer );
		// I'm doing this because I don't have access to an "Arrow" entity to pass to ClipContext's constructor
		const auto CollisionContext = ClipContext->collisionContext().CastTo<net::minecraft::world::phys::EntityCollisionContext*>();
		CollisionContext->descending( false );
		CollisionContext->entityBottom( Position.y );

		auto HitResult = Level->clip( ClipContext );
		if ( g_Java.GetEnv()->IsSameObject( HitResult->getType(), net::minecraft::world::phys::HitResult::Type::MISS() ) )
			HitResult = nullptr;
		else
			Vec3NextPosition = HitResult->getLocation();

		// Reversed EntityType and setPos
		const auto Width2 = 0.5 / 2.0;
		const auto Height = 0.5;
		Math::AABB BoundingBox( Position.x - Width2, Position.y, Position.z - Width2, Position.x + Width2, Position.y + Height, Position.z + Width2 );
		
		if ( Position.y >= -64.0 )
		{
			// Reversed findHitEntity
			auto ExpandedBoundingBox = BoundingBox;
			ExpandedBoundingBox.ExpandTowards( DeltaMovement ).Inflate( 1.0 );
			
			auto EntityHitResult = net::minecraft::world::entity::projectile::ProjectileUtil::getEntityHitResult( Level, LocalPlayer, Vec3Position, Vec3NextPosition, ExpandedBoundingBox.ToMinecraft(), nullptr );
			if ( !EntityHitResult && HitResult && g_Java.GetEnv()->IsSameObject( HitResult->getType(), net::minecraft::world::phys::HitResult::Type::ENTITY() ) )
				EntityHitResult = HitResult.CastTo<net::minecraft::world::phys::EntityHitResult*>();

			if ( EntityHitResult ) // Unfortunately I don't want to figure out how to do predicates from JNI so this isn't perfect
			{
				// Reversed canHitEntity and onHitEntity
				auto Entity = EntityHitResult->getEntity();
				if ( Entity->isSpectator() || !Entity->isAlive() || !Entity->isPickable() || g_Java.GetEnv()->IsSameObject( Entity->getType(), net::minecraft::world::entity::EntityType::ENDERMAN() ) ||
					 LocalPlayer->isPassengerOfSameVehicle( Entity ) || g_Java.GetEnv()->IsInstanceOf( Entity, net::minecraft::client::player::Player::GetThisClass() )
					 && !LocalPlayer->canHarmPlayer( Entity.CastTo<net::minecraft::client::player::Player*>() ) )
					EntityHitResult = nullptr;

				if ( EntityHitResult )
					removed = true;
			}
			else if ( HitResult )
			{
				// Reversed onHitBlock
				/*DeltaMovement.x() = Vec3NextPosition->x() - Position.x();
				DeltaMovement.y() = Vec3NextPosition->y() - Position.y();
				DeltaMovement.z() = Vec3NextPosition->z() - Position.z();

				Position -= DeltaMovement.Normalized() * 0.05000000074505806;*/

				// I'm not doing that because I just want the clipped position to be the endpoint
				DeltaMovement = Math::Vector::ZERO();
				Position = Vec3NextPosition;

				inGround = true;
			}
		}

		const auto NextPosition = Position + DeltaMovement;

		yRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.x, DeltaMovement.z ) ) );
		xRot = static_cast<float>( Math::RadToDeg( atan2( DeltaMovement.y, DeltaMovement.GetHorizontalDistance() ) ) );

		xRot = xRotO + Math::AngleNormalize( xRot - xRotO ) * 0.2f;
		yRot = yRotO + Math::AngleNormalize( yRot - yRotO ) * 0.2f;

		// Reversed updateFluidHeightAndDoFluidPushing
		static auto isInWater = []( net::minecraft::world::level::Level* Level, Math::AABB& BoundingBox ) // Passing BoundingBox by reference since it won't get used anymore
		{
			BoundingBox.Deflate( 0.001 );
			
			const auto MinX = static_cast<int>( floor( BoundingBox.Mins.x ) );
			const auto MaxX = static_cast<int>( ceil( BoundingBox.Maxs.x ) );
			const auto MinY = static_cast<int>( floor( BoundingBox.Mins.y ) );
			const auto MaxY = static_cast<int>( ceil( BoundingBox.Maxs.y ) );
			const auto MinZ = static_cast<int>( floor( BoundingBox.Mins.z ) );
			const auto MaxZ = static_cast<int>( ceil( BoundingBox.Maxs.z ) );

			if ( !reinterpret_cast<net::minecraft::world::level::LevelReader*>( Level )->hasChunksAt( MinX, MinY, MinZ, MaxX, MaxY, MaxZ ) )
				return false;

			const auto Water = net::minecraft::tags::FluidTags::WATER();
			const auto Block = net::minecraft::core::BlockPos::MutableBlockPos::init();
			for ( auto x = MinX; x < MaxX; ++x )
			{
				for ( auto y = MinY; y < MaxY; ++y )
				{
					for ( auto z = MinZ; z < MaxZ; ++z )
					{
						Block->set( x, y, z );

						const auto State = Level->getFluidState( Block );
						if ( State->is( Water ) )
						{
							if ( y + static_cast<double>( State->getHeight( Level, Block ) ) >= BoundingBox.Mins.y )
								return true;
						}
					}
				}
			}

			return false;
		};
		auto Inertia = 0.99f;
		if ( isInWater( Level, BoundingBox ) )
			Inertia = 0.6f;

		DeltaMovement *= Inertia;
		DeltaMovement.y -= 0.05000000074505806;

		Position = NextPosition;

		Trajectory.emplace_back( Position, removed ? HitType::Entity : inGround ? HitType::Block : HitType::None );
		
		if ( Position.y < -64.0 )
			break;
	}

	return Trajectory;
}

void DrawBowTrajectory( const Drawing::Context& Context, const BowTrajectory& Trajectory )
{
	if ( Trajectory.empty() )
		return;

	Drawing::Color Color( 255, 0, 0 );

	const auto& LastPoint = Trajectory.back();
	if ( LastPoint.Hit == HitType::Entity )
		Color = Drawing::Color( 0, 255, 0 );

	if ( LastPoint.Hit != HitType::None )
		DrawSphere( Context, LastPoint.Location, 0.5, Color );

	auto LastOnScreen = false;
	Drawing::Point LastLocation;
	for ( const auto& Point : Trajectory )
	{
		Drawing::Point Location;
		const auto OnScreen = WorldToScreen( Context, Point.Location, Location );

		if ( LastOnScreen && OnScreen )
			DrawLine( Color, LastLocation, Location );

		LastOnScreen = OnScreen;
		LastLocation = Location;
	}
}

BOOL WINAPI Hooks::wglSwapBuffers( HDC DC )
{
	if ( auto Minecraft = net::minecraft::client::Minecraft::getInstance() )
	{
		if ( auto LocalPlayer = Minecraft->player() )
		{
			auto Context = Drawing::Begin( Minecraft );
			
			auto Level = LocalPlayer->level();
			if ( g_Config.ShouldDrawCrystalHitboxes() )
				ESP( Context, LocalPlayer, Level );

			if ( g_Config.ShouldDrawBowTrajectory() )
				DrawBowTrajectory( Context, GetBowTrajectory( LocalPlayer, Level ) );

			End( Context );
		}
	}
	
	return reinterpret_cast<decltype( &wglSwapBuffers )>( owglSwapBuffers )( DC );
}