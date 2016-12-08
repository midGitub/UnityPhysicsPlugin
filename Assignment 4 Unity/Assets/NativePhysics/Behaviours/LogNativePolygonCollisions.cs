using UnityEngine;

namespace Humber.GAME205.NativePhysics
{
    [RequireComponent( typeof( PolygonNativePhysicsAdapter ) )]
    public class LogNativePolygonCollisions : MonoBehaviour
    {
        PolygonNativePhysicsAdapter polygonAdapter;

        public PolygonNativePhysicsAdapter PolygonAdapter
        {
            get { return polygonAdapter ?? ( polygonAdapter = GetComponent<PolygonNativePhysicsAdapter>() ); }
        }


        // Update is called once per frame
        void Update()
        {
            if ( PolygonAdapter.IsColliding() )
            {
                Debug.Log( PolygonAdapter.name + "is colliding!" );
            }
        }
    }
}