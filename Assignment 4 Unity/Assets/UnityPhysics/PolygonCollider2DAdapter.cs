using UnityEngine;

namespace Humber.GAME205.UnityPhysics
{
    [ExecuteInEditMode]
    public class PolygonCollider2DAdapter : MonoBehaviour
    {
        // Inspector/Public Fields
        public Polygon polygon;
        public new PolygonCollider2D collider;

        // Computed Properties
        public PolygonCollider2D Collider
        {
            get { return collider ?? ( collider = GetComponent<PolygonCollider2D>() ); }
        }
        public Polygon Polygon
        {
            get { return polygon ?? ( polygon = GetComponent<Polygon>() ); }
        }

        void Start()
        {
            UpdateColliderPoints();

            // Set up an event listener to update the Collider geometry whenever the Polygon changes.
            Polygon.VerticesChanged.AddListener( UpdateColliderPoints );
        }

        void UpdateColliderPoints()
        {
            if ( Collider != null && Polygon != null )
            {
                Collider.points = Polygon.Vertices;
            }
        }
    }
}
