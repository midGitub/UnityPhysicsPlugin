using UnityEngine;
using UnityEngine.Events;
using System;

namespace Humber.GAME205
{
    public class Polygon : MonoBehaviour
    {
        // Inspector/Public Fields
        [SerializeField] // This field will be visible in the inspector even though it is private to access in code.
        private Vector2[] vertices;

        // Events
        [Header( "Events" )]
        [Tooltip( "Whenever the vertices of this polygon are changed, this event will be invoked." )]
        public UnityEvent VerticesChanged;

        // Static
        // The default set of vertices to use if none is set should be a unit square.
        private static readonly Vector2[] DEFAULT_VERTICES = 
            new Vector2[] 
            { 
                new Vector2(  0.5f,  0.5f ),
                new Vector2( -0.5f,  0.5f ),
                new Vector2( -0.5f, -0.5f ),
                new Vector2(  0.5f, -0.5f )
            };

        // Computed Properties
        public Vector2[] Vertices
        {
            get { return vertices ?? ( vertices = DEFAULT_VERTICES ); }
            set
            {
                if ( value == null )
                {
                    throw new ArgumentNullException( "vertices" );
                }
                if ( value != vertices )
                {
                    vertices = value;
                    VerticesChanged.Invoke();
                }
            }
        }

        #region Unity Message Handlers

        // Will run any time the Polygon's inspector values are modified.
        void OnValidate()
        {
            if ( vertices == null )
            {
                vertices = DEFAULT_VERTICES;
            }
            VerticesChanged.Invoke();
        }

        #endregion
    }
}
