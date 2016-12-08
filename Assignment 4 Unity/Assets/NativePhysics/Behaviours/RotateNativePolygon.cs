using UnityEngine;
using System.Collections;

namespace Humber.GAME205.NativePhysics
{
    [RequireComponent( typeof( PolygonNativePhysicsAdapter ) )]
    public class RotateNativePolygon : MonoBehaviour
    {
        public float DegreesPerSecond = 90f;

        PolygonNativePhysicsAdapter polygonAdapter;
        public PolygonNativePhysicsAdapter PolygonAdapter
        {
            get { return polygonAdapter ?? ( polygonAdapter = GetComponent<PolygonNativePhysicsAdapter>() ); }
        }
        

        // Update is called once per frame
        void Update()
        {
            PolygonAdapter.Rotate( DegreesPerSecond * Mathf.Deg2Rad * Time.deltaTime );
        }
    }
}