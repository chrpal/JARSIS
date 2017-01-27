using System;

// Import base message types
using KukaRsi.Messages;

namespace KukaRsi
{
    /// <summary>
    /// Stores a message object for access from different threads
    /// </summary>
    /// <typeparam name="MessageToRobot">The used message template</typeparam>
    internal class MessagePipe<MessageToRobot>
        where MessageToRobot : BaseMessageToRobot
    {
        /// <summary>
        /// A lock to avoid synchronous access on the stored message
        /// </summary>
        private Object InternalLock;

        /// <summary>
        /// The stored message object
        /// </summary>
        private MessageToRobot CurrentMessage;

        /// <summary>
        /// Initializes a new MessagePipe
        /// </summary>
        public MessagePipe()
        {
            this.InternalLock = new Object();
        }

        /// <summary>
        /// Stores a message object for cross-thread accesss
        /// </summary>
        /// <remarks>This can be used on any thread</remarks>
        /// <param name="message">The stored message</param>
        /// <returns>Whether the message could be stored</returns>
        public bool Push(MessageToRobot message)
        {
            bool alreadyAssigned = false;
            lock (this.InternalLock)
            {
                // Check for previous stored message
                if (this.CurrentMessage == null)
                {
                    this.CurrentMessage = message;
                }
                else
                {
                    alreadyAssigned = true;
                }
            }
            return !alreadyAssigned;
        }

        /// <summary>
        /// Returns and resets the stored message object
        /// </summary>
        /// <remarks>This should be used on the communication thread</remarks>
        /// <returns></returns>
        public MessageToRobot Pull()
        {
            MessageToRobot message;
            lock (this.InternalLock)
            {
                message = this.CurrentMessage;
                this.CurrentMessage = null;
            }
            return message;
        }

    }
}
