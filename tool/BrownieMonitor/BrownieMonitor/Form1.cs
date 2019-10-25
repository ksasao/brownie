using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace BrownieMonitor
{
    public partial class Form1 : Form
    {
        string[] logBuffer = new string[30];
        int logBufferPointer = 0;
        string logPath = @"./log";

        BindingList<CommandItem> commandList = new BindingList<CommandItem>();
        public Form1()
        {
            InitializeComponent();
            InitializeUI();
//            RunCommand(commandList[0].Then);
        }
        private void InitializeUI()
        {
            Directory.CreateDirectory(logPath);
            this.textBoxCOMPort.Text = Properties.Settings.Default.COMPort;
            this.dataGridViewCommand.Columns[1].ContextMenuStrip = this.contextMenuStrip1;
            LoadCommand();
        }
        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string data = serialPort1.ReadExisting().Trim();
            if(data.StartsWith("[DETECTED]: "))
            {
                string message = data.Substring(12);
                this.Invoke(new Action(() => {
                    this.labelInfo.Text = message;
                }));
                foreach (var c in commandList)
                {
                    if(c.If == message)
                    {

                        Log($"Detected:{c.If}");
                        RunCommand(c.Then);
                        break;
                    }
                }
                Log(data);
            }
            if (data.StartsWith("[DISTANCE]: "))
            {
                string message = data.Substring(12);
                try
                {
                    int dist = 10 * (int)((Convert.ToSingle(message) + 2.5f) / 5);
                    if (dist > 100)
                    {
                        dist = 100;
                    }
                    else if (dist < 0)
                    {
                        dist = 0;
                    }
                    this.Invoke(new Action(() => {
                        this.labelInfo.Text = dist.ToString();
                    }));
                }
                catch
                {
                    // Nothing to do when error occured
                }
            }
        }

        private void Log(string data)
        {
            string log = DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss") + " " + data;
            string logfile = Path.Combine(logPath, DateTime.Now.ToString("yyyyMMdd_HH")+".log");
            File.AppendAllText(logfile, log+"\r\n");

            StringBuilder sb = new StringBuilder();
            int p = logBufferPointer;
            logBuffer[p] = log;
            for (int i=0; i < logBuffer.Length; i++)
            {
                if (logBuffer[p] != null)
                {
                    sb.AppendLine(logBuffer[p]);
                }
                p = (p - 1 + logBuffer.Length) % logBuffer.Length;
            }
            logBufferPointer = (logBufferPointer + 1) % logBuffer.Length;
            this.Invoke(new Action(()=>{
                this.textBoxLog.Text = sb.ToString();
            }));
        }

        private bool InitializeSerialPort()
        {
            string port = this.textBoxCOMPort.Text.Trim();
            try
            {

                this.serialPort1.PortName = port;
                this.serialPort1.BaudRate = 115200;
                this.serialPort1.Open();
                this.serialPort1.DataReceived += SerialPort1_DataReceived;
            }
            catch (System.IO.IOException)
            {
                Log($"Error: {port} not found.");
                return false;
            }
            Log($"Info: Connected to {port}.");
            return true;
        }

        private void LoadCommand()
        {
            string file = "command.tsv";
            string[] list = File.ReadAllLines(file);
            commandList.Clear();
            for(int i=0; i<list.Length; i++)
            {
                string[] data = list[i].Split('\t');
                if (data.Length == 2) {
                    string message = data[0].Trim();
                    string command = data[1].Trim();
                    if(message != "" && command != "")
                    {
                        commandList.Add(new CommandItem { If = message, Then = command });
                    }
                }
            }
            this.dataGridViewCommand.DataSource = commandList;
        }

        private void ButtonSave_Click(object sender, EventArgs e)
        {
            List<string> output = new List<string>();
            for(int i=0; i < commandList.Count; i++)
            {
                string message = commandList[i].If?.Trim();
                string command = commandList[i].Then?.Trim();
                if(message != null && command != null && message != "" && command != "")
                {
                    output.Add(message + "\t" + command);
                }
                File.WriteAllLines("command.tsv",output.ToArray());
            }
            LoadCommand();
        }

        private void RunCommand(string command)
        {
            Log($"Execute: {command}");
            Process cmd = new Process();
            cmd.StartInfo.FileName = "cmd.exe";
            cmd.StartInfo.RedirectStandardInput = true;
            cmd.StartInfo.RedirectStandardOutput = true;
            cmd.StartInfo.CreateNoWindow = true;
            cmd.StartInfo.UseShellExecute = false;
            cmd.Start();

            cmd.StandardInput.WriteLine(command);
            cmd.StandardInput.Flush();
            cmd.StandardInput.Close();
            cmd.WaitForExit();
            Console.WriteLine(cmd.StandardOutput.ReadToEnd());
        }

        private void TextBoxCOMPort_TextChanged(object sender, EventArgs e)
        {

        }

        private void ButtonConnect_Click(object sender, EventArgs e)
        {
            if (this.textBoxCOMPort.Enabled)
            {
                if (InitializeSerialPort())
                {
                    Properties.Settings.Default.COMPort = this.textBoxCOMPort.Text.Trim();
                    Properties.Settings.Default.Save();
                    this.textBoxCOMPort.Enabled = false;
                    this.buttonConnect.Text = "Disconnect";
                }
            }
            else
            {
                this.textBoxCOMPort.Enabled = true;
                this.buttonConnect.Text = "Connect";
                serialPort1.Close();
            }
        }

        private void ExecuteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var selected = this.dataGridViewCommand.SelectedCells;
            if (selected.Count == 1)
            {
                foreach (var m in selected)
                {
                    var item = (System.Windows.Forms.DataGridViewCell)m;
                    var command = item.FormattedValue.ToString()?.Trim();
                    if(command != null && command != "")
                    {
                        RunCommand(command);
                    }
                }
            }
        }
    }
}
