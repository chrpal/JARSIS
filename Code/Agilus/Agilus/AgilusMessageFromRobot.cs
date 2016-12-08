using System.Xml.Serialization;

// Import base message types
using KukaRsi.Messages;

namespace Agilus
{
    class AgilusMessageFromRobot : BaseMessageFromRobot
    {
        /// <summary>
        /// Gets or sets the current robot position as TCP coordinate
        /// </summary>
        [XmlElement("RIst")]
        public TcpCoordinate CurrentPosition { get; set; }

        /// <summary>
        /// Gets or sets the command action parameter
        /// </summary>
        [XmlElement("CmdAct")]
        public int CmdAct { get; set; }

        /// <summary>
        /// Empty constructor for XML deserialization
        /// </summary>
        public AgilusMessageFromRobot() : base()
        {

        }
    }
}
