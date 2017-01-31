using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

// Import the base messages
using KukaRsi.Messages;

namespace KukaRsi
{
    /// <summary>
    /// Provides a communication interface for KUKA robots via the RSI protocol
    /// </summary>
    /// <typeparam name="MessageToRobot">The template for messages to the robot</typeparam>
    /// <typeparam name="MessageFromRobot">The template for messages from the robot</typeparam>
    public sealed class KukaRobot<MessageToRobot, MessageFromRobot>
        where MessageToRobot : BaseMessageToRobot
        where MessageFromRobot : BaseMessageFromRobot
    {
        /// <summary>
        /// Notifies about received messages
        /// </summary>
        public event Action<MessageFromRobot> Received;
        /// <summary>
        /// Notifies about changes of the connection state (connected, not connected)
        /// </summary>
        public event Action<bool> ConnectionStateChanged;

        /// <summary>
        /// Indicates whether the KUKA robot is connected
        /// </summary>
        public bool IsConnected { get; private set; }

        // Local endpoint to listen on
        private IPEndPoint LocalEndpoint;
        // UDP communication client
        private UdpClient Communicator;

        // Communication thread
        private Thread CommunicationThread;
        // Communication stop flag
        private volatile bool ShouldStop;

        // Message serializer
        private MessageSerialization<MessageToRobot, MessageFromRobot> Serialization;
        // Cross-thread message pipe
        private MessagePipe<MessageToRobot> Pipe;

        // Default (empty) message to the robot
        private MessageToRobot DefaultMessage;

        /// <summary>
        /// Creates a new KUKA robot communication
        /// </summary>
        /// <remarks>
        /// This interface operates as a server and waits for incoming UDP requests from the robot.
        /// Therefor, the local endpoint parameter must be a local endpoint, not the robot endpoint!
        /// </remarks>
        /// <param name="localEndpoint">The local network interface to open the UDP server</param>
        /// <param name="defaultMessage">The default message for non-stop idle communication</param>
        public KukaRobot(IPEndPoint localEndpoint, MessageToRobot defaultMessage)
        {
            this.IsConnected = false;
            this.LocalEndpoint = localEndpoint;
            this.Serialization = new MessageSerialization<MessageToRobot, MessageFromRobot>();
            this.Pipe = new MessagePipe<MessageToRobot>();
            this.DefaultMessage = defaultMessage;
        }

        /// <summary>
        /// Starts listening for UDP (RSI) requests
        /// </summary>
        public void Start()
        {
            this.ShouldStop = false;
            // Use a background thread for communication
            this.CommunicationThread = new Thread(() => this.Run()) { IsBackground = true };
            this.CommunicationThread.Start();
        }

        /// <summary>
        /// Stops listening for UDP requests
        /// </summary>
        public void Stop()
        {
            this.ShouldStop = true;
            // Wait for the thread to stop correctly
            this.CommunicationThread.Join();
        }

        /// <summary>
        /// Communication loop
        /// </summary>
        private void Run()
        {
            EndPoint ep = null;
            IPEndPoint targetEndpoint = null;
            this.Communicator = new UdpClient();
            this.Communicator.Client.Bind(this.LocalEndpoint);
            

            byte[] data = new byte[512];
            int iLen = 0;
            // Reconnect until should stop
            while (!this.ShouldStop)
            {
                try
                {
                    
                    // Wait for the first message from the robot
                    this.Communicator.Client.ReceiveTimeout = 50;
                    // Receive
                    byte[] receivedBytes = this.Communicator.Receive(ref targetEndpoint);
                    Console.WriteLine("client connected");
                    // Decode
                    string receivedString = Encoding.ASCII.GetString(receivedBytes);
                    // Deserialize
                    MessageFromRobot receivedMessage = this.Serialization.Deserialize(receivedString);
                    this.RaiseReceived(receivedMessage);
                    // First received message indicates an established connection
                    this.SetConnectionState(true);
                    // Predefine for sending
                    MessageToRobot commandMessage, sendMessage;
                    string sendString;
                    byte[] sendBytes;
                    // Connect to the robot
                    this.Communicator.Connect(targetEndpoint);
                    this.Communicator.Client.ReceiveTimeout = 15;
                    // Message loop until should stop
                    while (!this.ShouldStop)
                    {
                        Console.WriteLine("Receiving data");
                        // Check for a stored message, otherwise use default
                        commandMessage = this.Pipe.Pull();
                        sendMessage = (commandMessage != null) ? commandMessage : this.DefaultMessage;
                        // Copy IPOC from the last received message
                        sendMessage.SetIpoc(receivedMessage.Ipoc);
                        // Serialize
                        sendString = this.Serialization.Serialize(sendMessage);
                        // Encode
                        sendBytes = Encoding.ASCII.GetBytes(sendString);
                        // Send
                        this.Communicator.Send(sendBytes, sendBytes.Length);
                        // Receive
                        receivedBytes = this.Communicator.Receive(ref targetEndpoint);
                        // Decode
                        receivedString = Encoding.ASCII.GetString(receivedBytes);
                        // Deserialize
                        receivedMessage = this.Serialization.Deserialize(receivedString);
                        this.RaiseReceived(receivedMessage);
                    }
                } 
                catch (SocketException)
                {
                    // Catch communication errors as well as timeouts => disconnected
                    this.SetConnectionState(false);
                }
            }
            this.Communicator.Close();
        }

        /// <summary>
        /// Sends a message to the robot
        /// </summary>
        /// <param name="message">The message to send</param>
        public void Send(MessageToRobot message)
        {
            this.Pipe.Push(message);
        }

        private void RaiseReceived(MessageFromRobot message)
        {
            // Raise in another thread instead of communication thread
            this.Received.BeginInvoke(message, this.Received.EndInvoke, null);
        }

        private void SetConnectionState(bool connectionState)
        {
            // Only modify if the state changed
            if (this.IsConnected != connectionState)
            {
                this.IsConnected = connectionState;
                if (this.ConnectionStateChanged != null)
                {
                    // Raise in another thread instead of communication thread
                    this.ConnectionStateChanged.BeginInvoke(connectionState, this.ConnectionStateChanged.EndInvoke, null);
                }
            }
        }
    }
}
