﻿

namespace Agilus
{
    /// <summary>
    /// Provides a generic store to access variables of any type thread-safe
    /// </summary>
    /// <typeparam name="StoredType">The type of the object to store</typeparam>
    internal class Locked<StoredType>
    {
        // Stored variable
        private StoredType StoredObject;

        // Access lock
        private object Lock;

        /// <summary>
        /// Creates a new locked variable
        /// </summary>
        /// <param name="initial">An optional initial value</param>
        public Locked(StoredType initial = default(StoredType))
        {
            this.Lock = new object();
            this.StoredObject = initial;
        }

        /// <summary>
        /// Sets the value of the locked variable
        /// </summary>
        /// <param name="value">The new variable value</param>
        public void Set(StoredType value)
        {
            lock (this.Lock)
            {
                this.StoredObject = value;
            }
        }

        /// <summary>
        /// Gets the value of the locked variable
        /// </summary>
        public StoredType Get()
        {
            lock (this.Lock)
            {
                return this.StoredObject;
            }
        }
    }
}
