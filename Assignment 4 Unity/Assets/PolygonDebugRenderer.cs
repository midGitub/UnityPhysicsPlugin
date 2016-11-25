#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;
using System.Linq;

namespace Humber.GAME205
{
    [ExecuteInEditMode]
    public class PolygonDebugRenderer : MonoBehaviour
    {
        // Inpsector/Public fields
        [Tooltip( "Should the debug draw appear in the Unity Editor's scene view?" )]
        public bool DebugDrawInSceneView = true;
        [Tooltip( "Should the debug draw appear when viewing the scene in play mode?" )]
        public bool DebugDrawInPlayMode = true;
        [Tooltip( "Should the debug draw appear in the Unity Player and Standalone builds of your Unity project?" )]
        public bool DebugDrawInBuild = false;
        [Tooltip( "The color to draw the polygon with." )]
        public Color DebugDrawColor = new Color( 1f, 0.4f, 0f, 0.75f );
        [Tooltip( "The color to draw the polygon with." )]
        public Color SelectedColor = new Color( 0f, 1f, 1f, 1f );
        [Tooltip( "The polygon to draw (defaults to the sibling Polygon component if null)." )]
        public Polygon[] polygons;

        // Static
        protected static Material lineMaterial;

        // Computed Properties
        public Polygon[] Polygons
        {
            // If polygons isn't set (IS NULL), set it to the collection of Polygon components in this GO 
            // and its children and return them.
            get { return polygons ?? ( polygons = GetComponentsInChildren<Polygon>() ); }
        }

        #region Unity Message Handlers

        protected virtual void Start()
        {
            // Reset the polygons list.
            polygons = GetComponentsInChildren<Polygon>();
        }

        protected virtual void OnRenderObject()
        {
#if UNITY_EDITOR
            if ( DebugDrawInPlayMode )
            {
                PlayModeDebugDraw();
            }
#else
            if ( DebugDrawInBuild )
            {
                PlayModeDebugDraw();
            }
#endif
        }

        protected virtual void OnTransformChildrenChanged()
        {
            // Reset the polygons list.
            polygons = GetComponentsInChildren<Polygon>();
        }

        // Will get called in the editor to draw into the scene view.
        protected virtual void OnDrawGizmos()
        {
            if ( DebugDrawInSceneView )
            {
                EditorDebugDraw();
            }
        }

        #endregion

        #region Debug Draw Methods

        protected virtual void EditorDebugDraw()
        {
            if ( Polygons == null )
            {
                return;
            }

            Color originalColor = Gizmos.color;
            Gizmos.color = DebugDrawColor;
            foreach ( var polygon in Polygons )
            {
                if ( !polygon.isActiveAndEnabled )
                {
                    continue;
                }
#if UNITY_EDITOR
                Gizmos.color = Selection.gameObjects.Contains( polygon.gameObject )
                    ? SelectedColor
                    : DebugDrawColor;
#endif
                for ( var i = 0; i < polygon.Vertices.Length; i++ )
                {
                    var iNext = ( i + 1 ) % polygon.Vertices.Length;

                    var vertex = polygon.Vertices[ i ];
                    var nextVertex = polygon.Vertices[ iNext ];

                    Gizmos.DrawLine(
                        polygon.transform.TransformPoint( vertex ),
                        polygon.transform.TransformPoint( nextVertex )
                    );
                }
            }
            Gizmos.color = originalColor;
        }

        protected virtual void PlayModeDebugDraw()
        {
            if ( Polygons == null )
            {
                return;
            }

            // Create/apply the line material.
            CreateLineMaterial();
            lineMaterial.SetPass( 0 );

            foreach ( var polygon in Polygons )
            {
                if ( !polygon.isActiveAndEnabled )
                {
                    continue;
                }

                // Set up the MVP matrix for this polygon.
                GL.PushMatrix();
                GL.MultMatrix( polygon.transform.localToWorldMatrix );

                // Batch the draw and send it to the GPU.
                GL.Begin( GL.LINES );
                {
                    for ( var i = 0; i < polygon.Vertices.Length; i++ )
                    {
                        var iNext = ( i + 1 ) % polygon.Vertices.Length;

                        var vertex = polygon.Vertices[ i ];
                        var nextVertex = polygon.Vertices[ iNext ];

                        GL.Color( DebugDrawColor );
                        GL.Vertex3( vertex.x, vertex.y, 0f );
                        GL.Vertex3( nextVertex.x, nextVertex.y, 0f );
                    }
                }
                GL.End();

                // Revert to the previous MVP matrix.
                GL.PopMatrix();
            }
        }
        
        protected void CreateLineMaterial()
        {
            if ( lineMaterial )
            {
                return;
            }

            // Unity has a built-in shader that is useful for drawing simple colored things.
            Shader shader = Shader.Find( "Hidden/Internal-Colored" );
            lineMaterial = new Material( shader );
            lineMaterial.hideFlags = HideFlags.HideAndDontSave;

            // Turn on alpha blending.
            lineMaterial.SetInt( "_SrcBlend", (int)UnityEngine.Rendering.BlendMode.SrcAlpha );
            lineMaterial.SetInt( "_DstBlend", (int)UnityEngine.Rendering.BlendMode.OneMinusSrcAlpha );

            // Turn off backface culling.
            lineMaterial.SetInt( "_Cull", (int)UnityEngine.Rendering.CullMode.Off );

            // Turn off depth writes.
            lineMaterial.SetInt( "_ZWrite", 0 );
        }

        #endregion
    }
}
