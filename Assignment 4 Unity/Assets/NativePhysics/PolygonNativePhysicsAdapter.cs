using UnityEngine;
using System;

namespace Humber.GAME205.NativePhysics
{
    public class PolygonNativePhysicsAdapter : MonoBehaviour
    {
        // Inspector/Public Fields
        public NativePhysicsWorld world;
        public Polygon polygon;
        [ReadOnly, SerializeField] int handle; // Set on Awake() by NativePhysics.PolygonCreate().

        [Header( "Physics Properties" )]
        public bool useGravity = false;
        public float mass = 1f;

        #region Computed Properties
        public int Handle
        {
            get { return handle; }
        }
        public Polygon Polygon
        {
            // If polygon isn't set (IS NULL), set it to the Polygon component of this GO and return it.
            get { return polygon ?? ( polygon = GetComponent<Polygon>() ); }
        }
        public Vector2[] Vertices
        {
            // Just get the vertices from the Polygon. No need to copy any data!
            get { return Polygon.Vertices; }
        }
        public float Mass
        {
            get { return World.PolygonGetMass( handle ); }
            set { World.PolygonSetMass( handle, value ); }
        }
        public float RotationalInertia
        {
            get { return World.PolygonGetRotationalInertia( handle ); }
        }
        public Vector2 Position
        {
            // Getter and setter to treat the Vector3 from the Transform as a Vector2 (the z-axis is only 
            // used for draw order in 2D) and to automatically update the native Polygon's position when 
            // this value gets changed.
            get { return (Vector2)Polygon.transform.localPosition; }
            set
            {
                Polygon.transform.localPosition = value;
                World.PolygonSetPosition( handle, value );
            }
        }
        public Vector2 Velocity
        {
            get { return World.PolygonGetVelocity( handle ); }
            set { World.PolygonSetVelocity( handle, value ); }
        }
        public float Rotation
        {
            // Getter and setter to treat the Quaternion from the Transform as a float (2D objects only 
            // rotate on the z-axis) and to automatically update the native Polygon's rotation when this 
            // value gets changed.
            get { return Mathf.Deg2Rad * Polygon.transform.localRotation.eulerAngles.z; }
            set
            {
                Polygon.transform.localRotation = Quaternion.Euler( 0f, 0f, Mathf.Rad2Deg * value );
                World.PolygonSetRotation( handle, value );
            }
        }
        public float RotationalVelocity
        {
            get { return World.PolygonGetRotationalVelocity( handle ); }
            set { World.PolygonSetRotationalVelocity( handle, value ); }
        }
        public NativePhysicsWorld World
        {
            // If world isn't set (IS NULL), set it to the NativePhysicsWorld component of this GO or 
            // any of its ancestors and return it.
            get { return world ?? ( world = GetComponentInParent<NativePhysicsWorld>() ); }
        }
        #endregion

        #region NativePhysicsWorld calls that get/set things in C++ land

        public void Translate( Vector2 dPosition )
        {
            World.PolygonTranslate( handle, dPosition );
        }

        public void Accelerate( Vector2 dVelocity )
        {
            World.PolygonAccelerate( handle, dVelocity );
        }

        public void Rotate( float dRadians )
        {
            World.PolygonRotate( handle, dRadians );
        }

        public void AccelerateRotation( float dRadiansPerSecond )
        {
            World.PolygonAccelerateRotation( handle, dRadiansPerSecond );
        }

        public bool IsColliding()
        {
            return World.IsPolygonColliding( handle );
        }

        #endregion

        #region Unity Message Handlers

        void Awake()
        {
            if ( World == null )
            {
                throw new InvalidOperationException( "Cannot start up a PolygonNativePhysicsAdapter that is not a child of a NativePhysicsWorld!" );
            }

            // Create the C++ instance representing this polygon.
            handle = World.PolygonCreate( Polygon.Vertices, Position, Rotation, mass, useGravity );

            // Set up an event listener to update the native polygon's geometry whenever it changes in C#.
            Polygon.ValuesChanged.AddListener( () => {
                World.PolygonSetVertices( handle, Polygon.Vertices );
            } );
        }

        void OnDestroy()
        {
            // Make sure not to leak any memory in the native code by destroying the C++ Polygon 
            // instance before this GameObject is removed from the scene. But if the scene is 
            // already gone, we don't need to worry about cleaning up (it would throw an exception!).
            if ( World.DoesNativeWorldExist )
            {
                World.PolygonDestroy( handle );
            }
        }

        void Update()
        {
            // Update our position and rotation based on any changes that the native physics engine 
            // has made during its physics steps.
            Polygon.transform.localPosition = World.PolygonGetPosition( handle );
            Polygon.transform.localRotation = Quaternion.Euler( 0f, 0f, Mathf.Rad2Deg * World.PolygonGetRotation( handle ) );

            //check if its not the static cube
            if(handle != 2)
            {
                if (World.IsPolygonColliding(handle))
                {
                    Debug.Log("Polygon " + handle + " collided");
                    Debug.Log("Velocity " + Velocity.y);
                }
            }
            
        }

        void OnValidate()
        {
            // Never allow the scaling to be adjusted.
            transform.localScale = new Vector3( 1f, 1f, 1f );
        }

        #endregion
    }
}
