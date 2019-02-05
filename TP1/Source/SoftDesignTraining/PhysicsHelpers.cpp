// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "PhysicsHelpers.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

PhysicsHelpers::PhysicsHelpers(UWorld* world)
: m_world(world)
{
}

PhysicsHelpers::~PhysicsHelpers()
{
}


bool PhysicsHelpers::CastRay( const FVector& start,const FVector& end, TArray<struct FHitResult>& outHits, bool drawDebug )
{
    if( m_world == nullptr )
        return false;

    //Draw ray
    if( drawDebug )
        DrawDebugLine(m_world, start, end, FColor::Green);

 
    //Multi line trace
    FCollisionObjectQueryParams objectQueryParams;
    objectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
    objectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
    objectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
    queryParams.bReturnPhysicalMaterial = true;

    m_world->LineTraceMultiByObjectType(outHits, start, end, objectQueryParams, queryParams);

    //Draw hits
    if( drawDebug )
    {
        for( int32 i = 0; i < outHits.Num(); ++i )
            DebugDrawHitPoint(outHits[i]);
    }

    return outHits.Num() > 0;
}


bool PhysicsHelpers::SphereCast(const FVector& start,const FVector& end, float radius, TArray<struct FHitResult>& outHits, bool drawDebug )
{
    if( m_world == nullptr )
        return false;

    if( drawDebug )
    {
        FVector center = (start + end) * 0.5f;

        float halfHeight;
        FVector dir;
        (end - start).ToDirectionAndLength(dir, halfHeight);

        halfHeight *= 0.5f;
        halfHeight += radius;


        FQuat rotation = dir.ToOrientationQuat();

        FQuat rotY = FQuat(rotation.GetRightVector(), HALF_PI );

        rotation *= rotY;

        DrawDebugCapsule(m_world, center, halfHeight, radius, rotation, FColor::Green);



        DrawDebugLine(m_world, start, end, FColor::Red);
    }
        


    FCollisionObjectQueryParams objectQueryParams; // All objects
    FCollisionShape collisionShape;
    collisionShape.SetSphere(radius);
    FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
    queryParams.bReturnPhysicalMaterial = true;

    m_world->SweepMultiByObjectType(outHits, start, end, FQuat::Identity, objectQueryParams, collisionShape, queryParams);


    //Draw hits
    if( drawDebug )
    {
        for( int32 i = 0; i < outHits.Num(); ++i )
            DebugDrawHitPoint(outHits[i]);
    }

    return outHits.Num() > 0;
}
bool PhysicsHelpers::SphereOverlap( const FVector& pos, float radius, TArray<struct FOverlapResult>& outOverlaps, bool drawDebug  )
{
    if( m_world == nullptr )
        return false;

    if( drawDebug )
        DrawDebugSphere( m_world, pos, radius, 24, FColor::Green );


    FCollisionObjectQueryParams objectQueryParams; // All objects
    FCollisionShape collisionShape;
    collisionShape.SetSphere(radius);
    FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
    queryParams.bReturnPhysicalMaterial = true;

    m_world->OverlapMultiByObjectType(outOverlaps, pos, FQuat::Identity, objectQueryParams, collisionShape, queryParams);

    //Draw overlap results
    if( drawDebug )
    {
        for( int32 i = 0; i < outOverlaps.Num(); ++i )
        {
            if( outOverlaps[i].GetComponent() )
                DebugDrawPrimitive(*(outOverlaps[i].GetComponent()));
        }
    }

    return outOverlaps.Num() > 0;
}

void PhysicsHelpers::DebugDrawHitPoint(const FHitResult& hit)
{
    DrawDebugPoint(m_world, hit.ImpactPoint, 10.0f, FColor::Red, false, 1.0f, 0);

    const float arrowLength = 20.0f;
    DrawDebugDirectionalArrow(m_world, hit.ImpactPoint, hit.ImpactPoint + hit.ImpactNormal * arrowLength, 2, FColor::Blue, false, 1.0f, 0, 2);
}

void PhysicsHelpers::DebugDrawPrimitive(const UPrimitiveComponent& primitive)
{
    FVector center = primitive.Bounds.Origin;
    FVector extent = primitive.Bounds.BoxExtent;

    DrawDebugBox(m_world, center, extent, FColor::Red );
}