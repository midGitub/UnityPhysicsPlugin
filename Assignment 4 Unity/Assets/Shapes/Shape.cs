using UnityEngine;
using UnityEngine.Events;
namespace Humber.GAME205
{
    public abstract class Shape : MonoBehaviour
    {
        // Events
        [Header("Events")]
        [Tooltip("Whenever the data of this shape is changed, this event will be invoked.")]
        public UnityEvent ValuesChanged;

        protected abstract void OnValidate();
    }
}