using UnityEngine;
using UnityEngine.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;

namespace Humber.GAME205.NativePhysics
{
    [Serializable]
    public class NativePolygonEvent : UnityEvent<PolygonNativePhysicsAdapter> { }

    public sealed class NativePhysicsWorld : MonoBehaviour
    {
        // Inspector/Public Fields
        [Tooltip( "Should the physics world clock stay in sync with Unity's update by passing in Time.deltaTime each frame?" )]
        public bool SyncWithUnityUpdate = true;
        [Tooltip( "If synchronizing with the Unity update, should Time.deltaTime or Time.unscaledDeltaTime be passed in?" )]
        public bool UseUnscaledTime = false;
        [Tooltip( "How long (in seconds) should each native physics timestep be?" )]
        public float FixedTimestepSeconds = 0.02f;
        [Tooltip( "Acceleration due to the force of gravity in m/s^2?" )]
        public float GravityAcceleration = -9.81f;

        // Properties
        public bool DoesNativeWorldExist { get; private set; }

        #region Unity Message Handlers

        void Awake()
        {
            NativePhysics.WorldStart( FixedTimestepSeconds, GravityAcceleration );
            DoesNativeWorldExist = true;
        }

        void Update()
        {
            if ( SyncWithUnityUpdate )
            {
                NativePhysics.WorldUpdate( UseUnscaledTime ? Time.unscaledDeltaTime : Time.deltaTime );
            }
        }

        void OnDestroy()
        {
            NativePhysics.WorldDestroy();
            DoesNativeWorldExist = false;
        }

        #endregion

        #region Native Wrapper Methods

        void ThrowExceptionIfNativeWorldDoesNotExist()
        {
            if ( !DoesNativeWorldExist )
            {
                throw new InvalidOperationException( "Cannot perform this operation because no native world is available at the moment." );
            }
        }

        public int PolygonCreate( IEnumerable<Vector2> vertices, Vector2 position, float rotation = 0f, float mass = 1f, bool useGravity = false )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();

            // Pack the collection of Vector2s for transport (this also copies the array).
            var transportVertices = vertices
            .Select( vertex => new TransportVector2( vertex ) )
            .ToArray();

            return NativePhysics.PolygonCreate( transportVertices, transportVertices.Length, new TransportVector2( position ), rotation, mass, useGravity );
        }
        
        public void PolygonDestroy( int handle )
        {
            NativePhysics.PolygonDestroy( handle );
        }

        public void PolygonSetVertices( int handle, IEnumerable<Vector2> vertices )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();

            // Pack the vertices as an array of NativeVector2s.
            var transportVertices = vertices
                .Select( vertex => new TransportVector2( vertex ) )
                .ToArray();

            NativePhysics.PolygonSetVertices( handle, transportVertices, transportVertices.Length );
        }

        public float PolygonGetMass( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetMass( handle );
        }

        public void PolygonSetMass( int handle, float mass )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonSetMass( handle, mass );
        }

        public Vector2 PolygonGetPosition( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetPosition( handle ).ToVector2();
        }
        
        public void PolygonSetPosition( int handle, Vector2 position )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonSetPosition( handle, new TransportVector2( position ) );
        }
        
        public void PolygonTranslate( int handle, Vector2 dPosition )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonTranslate( handle, new TransportVector2( dPosition ) );
        }

        public Vector2 PolygonGetVelocity( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetVelocity( handle ).ToVector2();
        }

        public void PolygonSetVelocity( int handle, Vector2 velocity )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonSetVelocity( handle, new TransportVector2( velocity ) );
        }

        public void PolygonAccelerate( int handle, Vector2 dVelocity )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonAccelerate( handle, new TransportVector2( dVelocity ) );
        }

        public float PolygonGetRotationalInertia( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetRotationalInertia( handle );
        }

        public float PolygonGetRotation( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetRotation( handle );
        }
        
        public void PolygonSetRotation( int handle, float rotation )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonSetRotation( handle, rotation );
        }
        
        public void PolygonRotate( int handle, float dRotation )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonRotate( handle, dRotation );
        }

        public float PolygonGetRotationalVelocity( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.PolygonGetRotationalVelocity( handle );
        }

        public void PolygonSetRotationalVelocity( int handle, float rotationalVelocity )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonSetRotationalVelocity( handle, rotationalVelocity );
        }

        public void PolygonAccelerateRotation( int handle, float dRotationalVelocity )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            NativePhysics.PolygonAccelerateRotation( handle, dRotationalVelocity );
        }

        public bool IsPolygonColliding( int handle )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();
            return NativePhysics.IsPolygonColliding( handle );
        }

        #endregion

        #region Native Physics DLL Import

        static class NativePhysics
        {
            const string DLL_NAME = "NativePhysics";

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldStart( float fixedTimestepSeconds, float gravityAcceleration = 0f );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldUpdate( float deltaTimeSeconds );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldDestroy();

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public static extern int PolygonCreate( TransportVector2[] vertices, int verticesLength, TransportVector2 position, float rotation = 0f, float mass = 1f, bool useGravity = false );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonDestroy( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetVertices( int handle, TransportVector2[] vertices, int length );
            
            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static float PolygonGetMass( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetMass( int handle, float mass );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static TransportVector2 PolygonGetPosition( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetPosition( int handle, TransportVector2 position );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonTranslate( int handle, TransportVector2 dPosition );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static TransportVector2 PolygonGetVelocity( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetVelocity( int handle, TransportVector2 velocity );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonAccelerate( int handle, TransportVector2 dVelocity );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static float PolygonGetRotationalInertia( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static float PolygonGetRotation( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetRotation( int handle, float rotation );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonRotate( int handle, float dRotation );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static float PolygonGetRotationalVelocity( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetRotationalVelocity( int handle, float rotationalVelocity );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonAccelerateRotation( int handle, float dRotationalVelocity );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static bool IsPolygonColliding( int handle );
        }

        #endregion
    }
}
