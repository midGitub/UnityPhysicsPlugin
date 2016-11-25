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

        // Properties
        public bool DoesNativeWorldExist { get; private set; }

        #region Unity Message Handlers

        void Awake()
        {
            NativePhysics.WorldStart( FixedTimestepSeconds );
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

        public int PolygonCreate( IEnumerable<Vector2> vertices, Vector2 position, float rotation = 0f )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();

            // Pack the collection of Vector2s for transport (this also copies the array).
            var transportVertices = vertices
            .Select( vertex => new TransportVector2( vertex ) )
            .ToArray();

            return NativePhysics.PolygonCreate( transportVertices, transportVertices.Length, new TransportVector2( position ), rotation );
        }
        
        public void PolygonDestroy( int handle )
        {
            NativePhysics.PolygonDestroy( handle );
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
        
        public void PolygonSetVertices( int handle, IEnumerable<Vector2> vertices )
        {
            ThrowExceptionIfNativeWorldDoesNotExist();

            // Pack the vertices as an array of NativeVector2s.
            var transportVertices = vertices
                .Select( vertex => new TransportVector2( vertex ) )
                .ToArray();

            NativePhysics.PolygonSetVertices( handle, transportVertices, transportVertices.Length );
        }

        #endregion

        #region Native Physics DLL Import

        static class NativePhysics
        {
            const string DLL_NAME = "NativePhysics";

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldStart( float fixedTimestepSeconds );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldUpdate( float deltaTimeSeconds );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void WorldDestroy();

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public static extern int PolygonCreate( TransportVector2[] vertices, int verticesLength, TransportVector2 position, float rotation = 0f );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonDestroy( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static TransportVector2 PolygonGetPosition( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetPosition( int handle, TransportVector2 position );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonTranslate( int handle, TransportVector2 dPosition );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static float PolygonGetRotation( int handle );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetRotation( int handle, float rotation );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonRotate( int handle, float dRotation );

            [DllImport( DLL_NAME, CallingConvention = CallingConvention.Cdecl )]
            public extern static void PolygonSetVertices( int handle, TransportVector2[] vertices, int length );
        }

        #endregion
    }
}
