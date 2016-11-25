using UnityEngine;

namespace Humber.GAME205.NativePhysics
{
    public struct TransportVector2
    {
        public float x;
        public float y;

        public TransportVector2( float x, float y )
        {
            this.x = x;
            this.y = y;
        }
        public TransportVector2( Vector2 vector )
        {
            x = vector.x;
            y = vector.y;
        }

        public Vector2 ToVector2()
        {
            return new Vector2( x, y );
        }
    }
}
