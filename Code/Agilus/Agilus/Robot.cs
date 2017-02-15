using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;

using KukaRsi;
using System.Threading;

namespace Agilus
{
    public class Robot
    {

        static void Main(string[] args)
        {
            KukaRsi.MessageSerialization<AgilusMessageToRobot, AgilusMessageFromRobot> ms = new KukaRsi.MessageSerialization<AgilusMessageToRobot, AgilusMessageFromRobot>();
            AgilusMessageToRobot msg = new AgilusMessageToRobot();


            String s = ms.Serialize(msg);
            

            double velocity = 100;
            double acceleration = 100;
            //AxisPosition target = new AxisPosition(0, -50, 90, 0, 90, 0);
            TcpCoordinate tcpTarget1 = new TcpCoordinate(445, 0, 900, -180, 0, -180);
            TcpCoordinate tcpTarget2 = new TcpCoordinate(445, 0, 800, -180, 0, -180);

/*            msg.DoubleParameters.Values[0] = tcpTarget.X;
            msg.DoubleParameters.Values[1] = tcpTarget.Y;
            msg.DoubleParameters.Values[2] = tcpTarget.Z;
            msg.DoubleParameters.Values[3] = tcpTarget.A;
            msg.DoubleParameters.Values[4] = tcpTarget.B;
            msg.DoubleParameters.Values[5] = tcpTarget.C;*/
            //IPAddress ipAddress = new IPAddress(2252147995); // für 134.61.13.28, 2252147996
            //Testen 127.0.0.1
            //IPAddress ipAddress = new IPAddress(new byte[4] { 134, 61, 13, 30});

            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, 41975);
            Robot agilus = new Robot(ipEndPoint);

            //agilus.MoveAbsoluteLinear(tcpTarget);

            //agilus.movePTP(1, new List<double> { 0, -50, 90, 0, 90, 0 }, 10, 100);
            agilus.MoveAbsoluteLinear(tcpTarget1);
            agilus.Start();

            //agilus.MoveAbsoluteLinear(tcpTarget);


            //agilus.MoveAbsolutePTP(target, velocity, acceleration);
            /*agilus.MoveAbsolutePTP(target, velocity, acceleration);
            agilus.MoveAbsolutePTP(target, velocity, acceleration);
            agilus.MoveAbsolutePTP(target, velocity, acceleration);
            agilus.MoveAbsolutePTP(target, velocity, acceleration);*/
            //agilus.AgilusMessageToRobot.Command.set = 100;

            int i = 0;
            Thread.Sleep(10);
            while (true)
            {
                i++;
                agilus.MoveAbsoluteLinear(tcpTarget2);
                Thread.Sleep(1000);
                agilus.MoveAbsoluteLinear(tcpTarget1);
                Thread.Sleep(1000);
                //agilus.MoveAbsoluteLinear(tcpTarget);
            }

            //agilus.Stop();
        }
        /// <summary>
        /// Called when a robot job was done
        /// </summary>
        public event Action JobDone;
        /// <summary>
        /// Called when the connection to the robot is maintained or lost
        /// </summary>
        public event Action<bool> ConnectionChanged;

        // RSI communication
        private KukaRobot<AgilusMessageToRobot, AgilusMessageFromRobot> KukaRobot;
        // Last received message
        private Locked<AgilusMessageFromRobot> LastMessage;

        /// <summary>
        /// Gets whether the connection to the robot is maintained
        /// </summary>
        public bool IsConnected { get; private set; }

        /// <summary>
        /// Creates a new Agilus robot control
        /// </summary>
        /// <param name="localEndpoint">The local endpoint to use for UDP connection</param>
        public Robot(IPEndPoint localEndpoint)
        {
            this.KukaRobot = new KukaRobot<AgilusMessageToRobot, AgilusMessageFromRobot>(localEndpoint, new AgilusMessageToRobot());
            this.KukaRobot.Received += this.onReceived;
            this.KukaRobot.ConnectionStateChanged += onConnectionStateChanged;
            this.LastMessage = new Locked<AgilusMessageFromRobot>();
        }

        private void onConnectionStateChanged(bool connected)
        {
            this.IsConnected = connected;
            /*this.ConnectionChanged.Invoke(connected);*/
        }

        /// <summary>
        /// Starts the robot connection
        /// </summary>
        public void Start()
        {
            this.KukaRobot.Start();
        }

        /// <summary>
        /// Stops the robot connection
        /// </summary>
        public void Stop()
        {
            this.KukaRobot.Stop();
        }

        private void onReceived(AgilusMessageFromRobot current)
        {
            AgilusMessageFromRobot previous = this.LastMessage.Get();
            if (previous != null)
            {
                // Compare current and previous message action
                if (previous.CmdAct != 0 && current.CmdAct == 0)
                {
                    // Transition from any other value to zero means a completed job

                    //TODO BÖÖÖSE!!!!
                    //this.JobDone.Invoke();
                }
            }
            // Store the received message as last one
            this.LastMessage.Set(current);
        }

        /// <summary>
        /// Moves the robot PTP to a specified target position
        /// </summary>
        /// <param name="target">The target as axis position</param>
        /// <param name="velocity">The velocity as percentage</param>
        /// <param name="acceleration">The acceleration as percentage</param>
        public void MoveAbsolutePTP(AxisPosition target, double velocity, double acceleration)
        {
            // Perform PTP movement in mode 1 (absolute axis)
            this.movePTP(1, target.EnumerateValues(), velocity, acceleration);
        }

        /// <summary>
        /// Moves the robot PTP to a specified target position
        /// </summary>
        /// <param name="target">The target as TCP coordinate</param>
        /// <param name="velocity">The velocity as percentage</param>
        /// <param name="acceleration">The acceleration as percentage</param>
        public void MoveAbsolutePTP(TcpCoordinate target, double velocity, double acceleration)
        {
            // Perform PTP movement in mode 3 (absolute position)
            this.movePTP(3, target.EnumerateValues(), velocity, acceleration);
        }

        /// <summary>
        /// Moves the robot PTP for a specified delta
        /// </summary>
        /// <param name="delta">The delta as relative axis position</param>
        /// <param name="velocity">The velocity as percentage</param>
        /// <param name="acceleration">The acceleration as percentage</param>
        public void MoveRelativePTP(AxisPosition delta, double velocity, double acceleration)
        {
            // Perform PTP movement in mode 2 (relative axis)
            this.movePTP(2, delta.EnumerateValues(), velocity, acceleration);
        }

        /// <summary>
        /// Moves the robot PTP for a specified delta
        /// </summary>
        /// <param name="delta">The delta as relative TCP coordinate</param>
        /// <param name="velocity">The velocity as percentage</param>
        /// <param name="acceleration">The acceleration as percentage</param>
        public void MoveRelativePTP(TcpCoordinate delta, double velocity, double acceleration)
        {
            // Perform PTP movement in mode 4 (relative position)
            this.movePTP(4, delta.EnumerateValues(), velocity, acceleration);
        }

        // Base method for PTP movement
        private void movePTP(int mode, IEnumerable<double> ptp, double velocity, double acceleration)
        {
            double[] parameters = new double[8];
            // Copy the target values to the parameters
            ptp.ToArray().CopyTo(parameters, 0);
            // Copy velocity and acceleration to the parameters
            parameters[6] = velocity; parameters[7] = acceleration;
            // Create the message
            AgilusMessageToRobot ptpMessage = new AgilusMessageToRobot(mode)
            { DoubleParameters = new DoubleParameters(parameters) };
            // Send the message
            this.KukaRobot.Send(ptpMessage);
        }

        /// <summary>
        /// Moves the robot linearly to a specified target position
        /// </summary>
        /// <param name="target">The target as TCP coordinate</param>
        public void MoveAbsoluteLinear(TcpCoordinate target)
        {
            // Perform linear movement in mode 5 (absolute)
            this.moveLinear(5, target.EnumerateValues());
        }

        /// <summary>
        /// Moves the robot linearly for a specified delta
        /// </summary>
        /// <param name="delta">The delta as relative TCP coordinate</param>
        public void MoveRelativeLinear(TcpCoordinate delta)
        {
            // Perform linear movement in mode 6 (relative)
            this.moveLinear(6, delta.EnumerateValues());
        }

        // Base method for linear movement
        private void moveLinear(int mode, IEnumerable<double> linear)
        {
            // Create the message
            AgilusMessageToRobot linearMessage = new AgilusMessageToRobot(mode)
            { DoubleParameters = new DoubleParameters(linear.ToArray()) };
            // Send the message
            this.KukaRobot.Send(linearMessage);
        }

        /// <summary>
        /// Stops the execution of the Agilus robot program
        /// </summary>
        public void StopExecution()
        {
            // Create the message
            AgilusMessageToRobot stopMessage = new AgilusMessageToRobot(100);
            // Send the message
            this.KukaRobot.Send(stopMessage);
        }

        /// <summary>
        /// Returns the current position of the robot as TCP coordinate
        /// </summary>
        /// <remarks>
        /// This method uses the last received message from the robot.
        /// Thereby, the result of this method can be wrong, if the robot is not connected.
        /// </remarks>
        public TcpCoordinate GetCurrentTcpPosition()
        {
            AgilusMessageFromRobot lastMessage = this.LastMessage.Get();
            return lastMessage.CurrentTcpPosition;
        }

        /// <summary>
        /// Returns the current position of the robot as Axis Position
        /// </summary>
        /// <remarks>
        /// This method uses the last received message from the robot.
        /// Thereby, the result of this method can be wrong, if the robot is not connected.
        /// </remarks>
        public AxisPosition GetCurrentAxisPosition()
        {
            AgilusMessageFromRobot lastMessage = this.LastMessage.Get();
            return lastMessage.CurrentAxisPosition;
        }
    }
}
