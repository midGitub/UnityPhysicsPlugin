using UnityEngine;
using System;

namespace Humber.GAME205
{
    public class Circle : Shape
    {
        // Static
        // The default set of vertices to use if none is set should be a unit square.
        private static readonly float DEFAULT_RADIUS = 1.0f;

        // Inspector/Public Fields
        [SerializeField] // This field will be visible in the inspector even though it is private to access in code.
        private float radius = DEFAULT_RADIUS;

        // Computed Properties
        public float Radius
        {
            get { return radius; }
            set
            {
                if (value != radius)
                {
                    if(value < 0)
                    {
                        radius = 0;
                    } else
                    {
                        radius = value;
                    }
                    
                    ValuesChanged.Invoke();
                }
            }
        }

        #region Unity Message Handlers

        // Will run any time the Circle's inspector values are modified.
        protected override void OnValidate()
        {
            ValuesChanged.Invoke();
        }

        #endregion
    }
}