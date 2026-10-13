using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using ExtendedSerialPort_NS;
using System.IO.Ports;

namespace RobotInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        ExtendedSerialPort serialPort1;
        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ExtendedSerialPort("COM5", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();
        }

        private void buttonSend_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }
        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            textBoxReception.Clear();
        }
        /*private void buttonSend_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }*/

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        private void SendMessage()
        {
            string text = textBoxEmission.Text;
            if (text != "" && text != "\n")
            {
                serialPort1.WriteLine(text);

                if (textBoxReception.Text != "")
                {
                    text = "\nRecu : " + text;
                }
                else
                {
                    text = "Recu : " + text;
                }
                textBoxReception.AppendText(text);
            }
            textBoxEmission.Clear();
        }
    }
}