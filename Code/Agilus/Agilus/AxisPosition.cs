using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Serialization;

namespace Agilus
{
    public class AxisPosition
    {
        /// <summary>
        /// Gets or sets the A1 axis value
        /// </summary>
        [XmlAttribute("A1")]
        public double A1 { get; set; }

        /// <summary>
        /// Gets or sets the A2 axis value
        /// </summary>
        [XmlAttribute("A2")]
        public double A2 { get; set; }

        /// <summary>
        /// Gets or sets the A3 axis value
        /// </summary>
        [XmlAttribute("A3")]
        public double A3 { get; set; }

        /// <summary>
        /// Gets or sets the A4 axis value
        /// </summary>
        [XmlAttribute("A4")]
        public double A4 { get; set; }

        /// <summary>
        /// Gets or sets the A5 axis value
        /// </summary>
        [XmlAttribute("A5")]
        public double A5 { get; set; }

        /// <summary>
        /// Gets or sets the A6 axis value
        /// </summary>
        [XmlAttribute("A6")]
        public double A6 { get; set; }

        /// <summary>
        /// Creates a new axis position (all values zero)
        /// </summary>
        public AxisPosition() { }

        /// <summary>
        /// Creates a new axis position with given values
        /// </summary>
        /// <param name="a1">The A1 axis value</param>
        /// <param name="a2">The A2 axis value</param>
        /// <param name="a3">The A3 axis value</param>
        /// <param name="a4">The A4 axis value</param>
        /// <param name="a5">The A5 axis value</param>
        /// <param name="a6">The A6 axis value</param>
        public AxisPosition(double a1, double a2, double a3, double a4, double a5, double a6)
        {
            this.A1 = a1;
            this.A2 = a2;
            this.A3 = a3;
            this.A4 = a4;
            this.A5 = a5;
            this.A6 = a6;
        }

        /// <summary>
        /// Returns an enumeration of all axis values
        /// </summary>
        public IEnumerable<double> EnumerateValues()
        {
            yield return this.A1;
            yield return this.A2;
            yield return this.A3;
            yield return this.A4;
            yield return this.A5;
            yield return this.A6;
        }

        /// <summary>
        /// Transforms each axis value from radiant to degrees and puts them into a new position
        /// </summary>
        public AxisPosition ToDegrees()
        {
            return new AxisPosition(
                this.A1 * 180 / Math.PI,
                this.A2 * 180 / Math.PI,
                this.A3 * 180 / Math.PI,
                this.A4 * 180 / Math.PI,
                this.A5 * 180 / Math.PI,
                this.A6 * 180 / Math.PI
            );
        }

        /// <summary>
        /// Stringifies the axis position values
        /// </summary>
        public override string ToString()
        {
            return "[ " + string.Join(", ", this.EnumerateValues().ToArray()) + " ]";
        }
    }
}
