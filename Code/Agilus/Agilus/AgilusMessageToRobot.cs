using System.Xml.Serialization;

// Import base message types
using KukaRsi.Messages;

namespace Agilus
{
    class AgilusMessageToRobot
    {
        /// <summary>
        /// The Modemo protocol identifier
        /// </summary>
        private const string MESSAGE_TYPE = "AgilusCmdMode";

        /// <summary>
        /// Gets or sets the command number
        /// </summary>
        [XmlElement("CmdNew")]
        public int? Command { get; set; }

        /// <summary>
        /// Gets or sets the double parameters
        /// </summary>
        [XmlElement("PCReal")]
        public DoubleParameters DoubleParameters { get; set; }

        /// <summary>
        /// Gets or sets the int parameters
        /// </summary>zie
        ///[XmlElement("PCInt")]
        ///public IntParameters IntParameters { get; set; }

        /// <summary>
        /// Gets or sets the bool parameters
        /// </summary>
        [XmlElement("PCBool")]
        public BoolParameters BoolParameters { get; set; }

        /// <summary>
        /// Gets or sets the position correction values
        /// </summary>
        [XmlElement("Corr")]
        public TcpCoordinate PositionCorrection { get; set; }

        /// <summary>
        /// Creates a new message to the robot with the Modemo template
        /// </summary>
        public AgilusMessageToRobot() : base(MESSAGE_TYPE)
        {
            this.Command = null;
            this.DoubleParameters = null;
            this.IntParameters = null;
            this.BoolParameters = null;
            this.PositionCorrection = null;
        }

        /// <summary>
        /// Creates a new message to the robot with the Modemo template
        /// </summary>
        /// <param name="command">The command number</param>
        public AgilusMessageToRobot(int? command) : base(MESSAGE_TYPE)
        {
            this.Command = command;
            this.DoubleParameters = null;
            this.IntParameters = null;
            this.BoolParameters = null;
            this.PositionCorrection = null;
        }

        /* 
         * By default, the XmlSerializer would serialize each property of this class.
         * Since we only want to serialize the provided parameters, we need to implement these helper functions.
         * Source: http://stackoverflow.com/a/5818571
         */

        #region | Serialization helpers |

        /// <summary>
        /// Indicates whether the Command should be serialized
        /// </summary>
        public bool ShouldSerializeCommand()
        {
            return this.Command.HasValue;
        }

        /// <summary>
        /// Indicates whether the DoubleParameters should be serialized
        /// </summary>
        public bool ShouldSerializeDoubleParameters()
        {
            return this.DoubleParameters != null;
        }

        /// <summary>
        /// Indicates whether the IntParameters should be serialized
        /// </summary>
        public bool ShouldSerializeIntParameters()
        {
            return this.IntParameters != null;
        }

        /// <summary>
        /// Indicates whether the BoolParameters should be serialized
        /// </summary>
        public bool ShouldSerializeBoolParameters()
        {
            return this.BoolParameters != null;
        }

        /// <summary>
        /// Indicates whether the PositionCorrection should be serialized
        /// </summary>
        public bool ShouldSerializePositionCorrection()
        {
            return this.PositionCorrection != null;
        }

        #endregion

    }
}
