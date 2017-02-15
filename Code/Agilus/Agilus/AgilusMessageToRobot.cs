using System.Xml.Serialization;

// Import base message types
using KukaRsi.Messages;
using System;


namespace Agilus
{
    public class AgilusMessageToRobot : BaseMessageToRobot
    {
        /// <summary>
        /// The Modemo protocol identifier
        /// </summary>
        private const string MESSAGE_TYPE = "RC4CmdMode";


        [XmlElement("EStr")]
        public String EStr { get; set; }

        /// <summary>
        /// Gets or sets the command number
        /// </summary>
        [XmlElement("CmdNew")]
        public int Command { get; set; }

        /// <summary>
        /// Gets or sets the double parameters
        /// </summary>
        [XmlElement("PCReal")]
        public DoubleParameters DoubleParameters { get; set; }

        [XmlElement("Stop")]
        public int Stop { get; set; }

        /// <summary>
        /// Creates a new message to the robot with the Modemo template
        /// </summary>
        public AgilusMessageToRobot() : base(MESSAGE_TYPE)
        {
            this.Command = 0;
            this.DoubleParameters = new DoubleParameters();
            this.DoubleParameters.Values = new double[] { 0, 0, 0, 0, 0, 0, 0, 0 };
            this.Stop = 0;
        }

        /// <summary>
        /// Creates a new message to the robot with the Modemo template
        /// </summary>
        /// <param name="command">The command number</param>
        public AgilusMessageToRobot(int command) : base(MESSAGE_TYPE)
        {
            this.Command = command;
            this.DoubleParameters = new DoubleParameters();
            this.DoubleParameters.Values = new double[] { 0, 0, 0, 0, 0, 0, 0, 0 };
            this.Stop = 0;
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
            return true;
        }

        /// <summary>
        /// Indicates whether the DoubleParameters should be serialized
        /// </summary>
        public bool ShouldSerializeDoubleParameters()
        {
            return this.DoubleParameters != null;
        }

        #endregion

    }
}
