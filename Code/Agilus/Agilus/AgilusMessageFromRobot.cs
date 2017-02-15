using System.Xml.Serialization;

// Import base message types
using KukaRsi.Messages;

namespace Agilus
{
    public class AgilusMessageFromRobot : BaseMessageFromRobot
    {
        /// <summary>
        /// Gets or sets the current robot position as TCP coordinate
        /// </summary>
        [XmlElement("RIst")]
        public TcpCoordinate CurrentTcpPosition { get; set; }

        [XmlElement("RSol")]
        public TcpCoordinate SetTcpPosition { get; set; }

        /// <summary>
        ///  Gets or sets the current robot position as Axis Position
        /// </summary>
        [XmlElement("AIPos")]
        public AxisPosition CurrentAxisPosition { get; set; }

        [XmlElement("ASPos")]
        public AxisPosition SetAxisPosition { get; set; }

        /*[XmlElement("MACur")]
        public AxisPosition CurrentMotorCurrent { get; set; }

        [XmlElement("Delay")]
        public int Delay { get; set; }*/

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
