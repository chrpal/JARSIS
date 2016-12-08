using System.Xml.Serialization;

namespace KukaRsi.Messages
{
    /// <summary>
    /// Base class for messages from the KUKA robot
    /// </summary>
    /// <remarks>
    /// This handles the XML deserialization. Inherit from this class and add properties fitting the elements from your RSI protocol with their corresponding names as XmlElement attributes.
    /// </remarks>
    public abstract class BaseMessageFromRobot
    {
        /// <summary>
        /// Gets or sets the used RSI protocol type
        /// </summary>
        [XmlAttribute("Type")]
        public string Type { get; set; }

        /// <summary>
        /// Gets or sets the current IPOC
        /// </summary>
        [XmlElement("IPOC")]
        public string Ipoc { get; set; }

        /// <summary>
        /// Empty constructor for XML deserialization
        /// </summary>
        public BaseMessageFromRobot()
        {
            this.Type = string.Empty;
            this.Ipoc = string.Empty;
        }
    }
}
