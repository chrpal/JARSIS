using System.IO;
using System.Xml;
using System.Xml.Serialization;

// Import base message types
using KukaRsi.Messages;

namespace KukaRsi
{
    /// <summary>
    /// Provides the possibility to serialize and deserialize messages from / to the robot
    /// </summary>
    /// <typeparam name="MessageToRobot">The template for messages to the robot</typeparam>
    /// <typeparam name="MessageFromRobot">The template for messages from the robot</typeparam>
    public class MessageSerialization<MessageToRobot, MessageFromRobot>
        where MessageToRobot : BaseMessageToRobot
        where MessageFromRobot : BaseMessageFromRobot
    {
        // XmlSerializer and settings (no namespace, no xml declaration) for message serialization
        private XmlSerializer Serializer;
        private XmlSerializerNamespaces EmptyNamespaces;
        private XmlWriterSettings Settings;

        // XmlSerializer for message deserialization
        private XmlSerializer Deserializer;

        public MessageSerialization()
        {
            // Initialize serialization part
            this.Serializer = new XmlSerializer(typeof(MessageToRobot), new XmlRootAttribute("Sen"));
            this.EmptyNamespaces = new XmlSerializerNamespaces(new[] { XmlQualifiedName.Empty });
            this.Settings = new XmlWriterSettings() { Indent = true, OmitXmlDeclaration = true };
            // Initialize deserialization part
            this.Deserializer = new XmlSerializer(typeof(MessageFromRobot), new XmlRootAttribute("Rob"));
        }

        /// <summary>
        /// Deserializes a received message string
        /// </summary>
        /// <param name="serialized">The message string</param>
        /// <returns>The message object</returns>
        public MessageFromRobot Deserialize(string serialized)
        {
            using (TextReader stream = new StringReader(serialized))
            {
                return (MessageFromRobot) this.Deserializer.Deserialize(stream);
            }
        }

        /// <summary>
        /// Serializes a message object
        /// </summary>
        /// <param name="instance">The message object</param>
        /// <returns>The message string</returns>
        public string Serialize(MessageToRobot instance)
        {
            using (TextWriter stream = new StringWriter())
            using (XmlWriter writer = XmlWriter.Create(stream, this.Settings))
            {
                this.Serializer.Serialize(writer, instance, this.EmptyNamespaces);
                return stream.ToString();
            }
        }

    }
}
