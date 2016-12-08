using System.Collections.Generic;
using System.Linq;
using System.Xml.Serialization;

namespace Agilus
{
    class TcpCoordinate
    {
        /// <summary>
        /// Gets or sets the X coordinate value
        /// </summary>
        [XmlAttribute("X")]
        public double X { get; set; }

        /// <summary>
        /// Gets or sets the Y coordinate value
        /// </summary>
        [XmlAttribute("Y")]
        public double Y { get; set; }

        /// <summary>
        /// Gets or sets the Z coordinate value
        /// </summary>
        [XmlAttribute("Z")]
        public double Z { get; set; }

        /// <summary>
        /// Gets or sets the A coordinate value
        /// </summary>
        [XmlAttribute("A")]
        public double A { get; set; }

        /// <summary>
        /// Gets or sets the B coordinate value
        /// </summary>
        [XmlAttribute("B")]
        public double B { get; set; }

        /// <summary>
        /// Gets or sets the C coordinate value
        /// </summary>
        [XmlAttribute("C")]
        public double C { get; set; }

        /// <summary>
        /// Creates a new TCP coordinate (all values zero)
        /// </summary>
        public TcpCoordinate() { }

        /// <summary>
        /// Creates a new TCP coordinate with given coordinate values
        /// </summary>
        /// <param name="x">The X coordinate value</param>
        /// <param name="y">The Y coordinate value</param>
        /// <param name="z">The Z coordinate value</param>
        /// <param name="a">The A coordinate value</param>
        /// <param name="b">The B coordinate value</param>
        /// <param name="c">The C coordinate value</param>
        public TcpCoordinate(double x, double y, double z, double a, double b, double c)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
            this.A = a;
            this.B = b;
            this.C = c;
        }

        /// <summary>
        /// Returns an enumeration of all TCP coordinate component values
        /// </summary>
        public IEnumerable<double> EnumerateValues()
        {
            yield return this.X;
            yield return this.Y;
            yield return this.Z;
            yield return this.A;
            yield return this.B;
            yield return this.C;
        }

        /// <summary>
        /// Stringifies the TCP coordinate component values
        /// </summary>
        public override string ToString()
        {
            return "[ " + string.Join(", ", this.EnumerateValues().ToArray()) + " ]";
        }
    }
}
