using System;
using System.Globalization;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;


namespace Agilus
{
    public class DoubleParameters : IXmlSerializable /* Handle xml serialization manually */
    {
        /// <summary>
        /// Gets or sets the parameter values
        /// </summary>
        public double[] Values { get; set; }

        /// <summary>
        /// Creates an empty set of double parameters
        /// </summary>
        public DoubleParameters()
        {
            this.Values = new double[0];
        }

        /// <summary>
        /// Creates a set of double parameters from given values
        /// </summary>
        /// <param name="values">The parameter values</param>
        public DoubleParameters(params double[] values)
        {
            this.Values = values;
        }

        #region | Manual XML serialization |

        public XmlSchema GetSchema()
        {
            // Do not provide a schema
            return null;
        }

        [Obsolete("Not supported")]
        public void ReadXml(XmlReader reader)
        {
            // XML deserialization is not required
            throw new NotSupportedException("Deserialization not supported");
        }

        public void WriteXml(XmlWriter writer)
        {
            for (int i = 1; i <= this.Values.Length; i++)
            {
                writer.WriteAttributeString("pr" + i.ToString(), this.Values[i - 1].ToString("F4", CultureInfo.InvariantCulture));
            }
        }

        #endregion // Manual XML serialization
    }
}
