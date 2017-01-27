using System.Xml.Serialization;

namespace KukaRsi.Messages
{
    /// <summary>
    /// Base class for messages to the KUKA robot
    /// </summary>
    /// <remarks>
    /// This handles the XML serialization. Inherit from this class and add properties fitting the elements from your RSI protocol with their corresponding names as XmlElement attributes.
    /// </remarks>
    public abstract class BaseMessageToRobot
    {
        /// <summary>
        /// Gets or sets the used RSI protocol
        /// </summary>
        [XmlAttribute("Type")]
        public string Type { get; set; }

        /// <summary>
        /// Gets or sets the IPOC
        /// </summary>
        [XmlElement("IPOC")]
        public string Ipoc { get; set; }

        /// <summary>
        /// Base constructor to assign a RSI protocol type
        /// </summary>
        /// <param name="type">The used RSI protocol</param>
        public BaseMessageToRobot(string type)
        {
            this.Type = type;
            this.Ipoc = string.Empty;
        }

        /// <summary>
        /// Sets the IPOC of this message to the robot
        /// </summary>
        /// <remarks>This should be used to mirror IPOC from a received message</remarks>
        /// <param name="ipoc">The requested IPOC</param>
        public void SetIpoc(string ipoc)
        {
            this.Ipoc = ipoc;
        }
    }
}
