using System;
using System.IO.Ports;
using System.Windows.Forms;



/*
 * 
 * 
 * 제작자 : OH SUN CHUL
 * 컴파일러 : Visual Studio
 * 사용언어 : C#
 * 
 * RC카 제어용 PC 콘솔 프로그램입니다.
 * 
 * 
 * 
 * 
 *
 *
 */



namespace Serial_Communication
{
    public partial class Form1 : Form
    {



        const int low = 0;
        const int off = 0;
        const int OFF = 0;
        const int LOW = 0;

        const int HIGH = 1;
        const int high = 1;
        const int on = 1;
        const int ON = 1;

        string TxData;

        string M1_Scroll_Value;
        string M2_Scroll_Value;
        string M3_Scroll_Value;
        string M4_Scroll_Value;

        char[] TxBuffer = new char[10];
        int temp_keycode;
        char[] TEMP_KEYCODE = new char[10];
        bool caps_lock_flag;

        int w_key_flag = off;

        int a_key_flag = off;
        int s_key_flag = off;
        int d_key_flag = off;
        int x_key_flag = off;

        int trans_key_w_state = off;
        int trans_key_x_state = off;
        int trans_key_s_state = off;
        int trans_key_q_state = off;
        int trans_key_e_state = off;
        int trans_key_z_state = off;
        int trans_key_c_state = off;

        int m1_flag = off;
        int m2_flag = off;
        int m3_flag = off;
        int m4_flag = off;

        int Dial_Flag = ON;

        char[] Key_Event_Data = new char[10];

        string KEY_DATA = "";

        string trans_key_data = "";

        string trackbar_m;
        string trackbar_m1;
        string trackbar_m2;
        string trackbar_m3;
        string trackbar_m4;




        private static DateTime Delay(int MS)
        {
          
            
            DateTime ThisMoment = DateTime.Now;
            TimeSpan duration = new TimeSpan(0, 0, 0, 0, MS);
            DateTime AfterWards = ThisMoment.Add(duration);

            while (AfterWards >= ThisMoment)
            {
                System.Windows.Forms.Application.DoEvents();
                ThisMoment = DateTime.Now;
            }

            return DateTime.Now;
        }


        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {


            string[] PORT_NAME = SerialPort.GetPortNames();
            comboBox_com.Items.AddRange(PORT_NAME);


        }

        private void richTextBox_Console_TextChanged(object sender, EventArgs e)
        {
          
            richTextBox_Console.SelectionStart = richTextBox_Console.TextLength;
            richTextBox_Console.ScrollToCaret();

        }
        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {


            if (checkBox_Car_Controller_Actiavte.Checked)
            {


                if (e.KeyCode == Keys.W)
                {

                    w_key_flag = on;

                    if (w_key_flag == on && a_key_flag == off &&

                        s_key_flag == off && d_key_flag == off && x_key_flag == off)
                    {

                        trans_key_data = "W";

                    }


                    if (w_key_flag == on && a_key_flag == on &&

                       s_key_flag == off && d_key_flag == off && x_key_flag == off)
                    {


                        trans_key_data = "Q";


                    }

                    if (w_key_flag == on && a_key_flag == off &&

                   s_key_flag == off && d_key_flag == on && x_key_flag == off)
                    {


                        trans_key_data = "E";


                    }

                    if (w_key_flag == on && a_key_flag == off &&

                  s_key_flag == off && d_key_flag == off && x_key_flag == on)
                    {

                        trans_key_data = "S";

                    }

                    button_W.Enabled = false;

                }

                else if (e.KeyCode == Keys.A)
                {


                    a_key_flag = on;

                    if (w_key_flag == on && a_key_flag == on &&

                           s_key_flag == off && d_key_flag == off && x_key_flag == off)
                    {

                        trans_key_data = "Q";

                    }

                    if (w_key_flag == off && a_key_flag == on &&

                       s_key_flag == off && d_key_flag == off && x_key_flag == on)
                    {


                        trans_key_data = "Z";

                    }

                    if (w_key_flag == off && a_key_flag == on &&

                     s_key_flag == off && d_key_flag == off && x_key_flag == off)
                    {


                        trans_key_data = "S";

                    }

                    button_A.Enabled = false;

                }

                else if (e.KeyCode == Keys.S)
                {

                    s_key_flag = on;

                    trans_key_data = "S";

                    button_S.Enabled = false;

                }

                else if (e.KeyCode == Keys.D)
                {

                    d_key_flag = on;

                    if (w_key_flag == on && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == on && x_key_flag == off)
                    {

                        trans_key_data = "E";


                    }

                    if (w_key_flag == off && a_key_flag == off &&

                       s_key_flag == off && d_key_flag == on && x_key_flag == on)
                    {

                        trans_key_data = "C";

                    }
                    if (w_key_flag == off && a_key_flag == off &&

                    s_key_flag == off && d_key_flag == on && x_key_flag == off)
                    {

                        trans_key_data = "S";

                    }

                    button_D.Enabled = false;


                }
                else if (e.KeyCode == Keys.X)
                {

                    x_key_flag = on;

                    if (w_key_flag == off && a_key_flag == off &&

                        s_key_flag == off && d_key_flag == off && x_key_flag == on)
                    {

                        trans_key_data = "X";

                    }

                    if (w_key_flag == off && a_key_flag == on &&

                               s_key_flag == off && d_key_flag == off && x_key_flag == on)
                    {

                        trans_key_data = "Z";


                    }
                    if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == on && x_key_flag == on)
                    {


                        trans_key_data = "C";

                    }

                    if (w_key_flag == on && a_key_flag == off &&

                      s_key_flag == off && d_key_flag == off && x_key_flag == on)
                    {

                        trans_key_data = "S";

                    }
                    button_X.Enabled = false;


                }

                // LED KEY BUTTON //
                else if (e.KeyCode == Keys.H)
                {

                    richTextBox_Console.Text += "LD1 : Toggle\n";
                    button_H.Enabled = false;

                    if (serialPort1.IsOpen)
                    {

                        serialPort1.Write("LD1\n");

                    }

                }
                else if (e.KeyCode == Keys.J)
                {

                    richTextBox_Console.Text += "LD2 : Toggle\n";
                    button_J.Enabled = false;

                    if (serialPort1.IsOpen)
                    {

                        serialPort1.Write("LD2\n");

                    }

                }

                else if (e.KeyCode == Keys.K)
                {

                    richTextBox_Console.Text += "LD3 : Toggle\n";
                    button_K.Enabled = false;

                    if (serialPort1.IsOpen)
                    {

                        serialPort1.Write("LD3\n");

                    }

                }

                else if (e.KeyCode == Keys.L)
                {

                    richTextBox_Console.Text += "LD4 : Toggle\n";
                    button_L.Enabled = false;

                    if (serialPort1.IsOpen)
                    {

                        serialPort1.Write("LD4\n");

                    }

                }

            }

                // Motor Power Adjust //

                if(checkBox_Power_Controller_Activate.Checked)
                { 
                           
                            if ( e.KeyCode == Keys.N) // Motor Power Decrease.
                            {


                                button_N.Enabled = false;

                    if (trackBar_m.Value > trackBar_m.Minimum)
                        trackBar_m.Value--;


                                Delay(100);
                                trackbar_m = Convert.ToString(trackBar_m.Value);
                                label_m.Text = trackbar_m;


                                trackBar_m1.Value = trackBar_m.Value;
                                label_m1.Text = trackbar_m;
                                trackBar_m2.Value = trackBar_m.Value;
                                label_m2.Text = trackbar_m;
                                trackBar_m3.Value = trackBar_m.Value;
                                label_m3.Text = trackbar_m;
                                trackBar_m4.Value = trackBar_m.Value;
                                label_m4.Text = trackbar_m;


                                if (serialPort1.IsOpen)
                                {

                                    trackbar_m = "5:" + trackbar_m + "\n";

                                    serialPort1.Write(trackbar_m);



                                }
                                Delay(100);

                            }

                            else if (e.KeyCode == Keys.M ) // Motor Power Increase.
                            {


                                button_M.Enabled = false;

                                if(trackBar_m.Value < trackBar_m.Maximum)
                                trackBar_m.Value++; 

                                Delay(100);
                                trackbar_m = Convert.ToString(trackBar_m.Value);
                                label_m.Text = trackbar_m;


                                trackBar_m1.Value = trackBar_m.Value;
                                label_m1.Text = trackbar_m;
                                trackBar_m2.Value = trackBar_m.Value;
                                label_m2.Text = trackbar_m;
                                trackBar_m3.Value = trackBar_m.Value;
                                label_m3.Text = trackbar_m;
                                trackBar_m4.Value = trackBar_m.Value;
                                label_m4.Text = trackbar_m;


                                if (serialPort1.IsOpen)
                                {

                                    trackbar_m = "5:" + trackbar_m + "\n";

                                    serialPort1.Write(trackbar_m);


                                }
                                Delay(100);


                            }

                }

        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {

                    if (checkBox_Car_Controller_Actiavte.Checked)
                    {
                        if (e.KeyCode == Keys.W)
                        {

                            w_key_flag = off;

                            if (w_key_flag == off && a_key_flag == off &&

                             s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {


                                trans_key_data = "S";
                                //  textBox_KeyDir.Text = trans_key_data;

                            }

                            if (w_key_flag == off && a_key_flag == on &&

                              s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {


                                trans_key_data = "S";
                                //   textBox_KeyDir.Text = trans_key_data;

                            }

                            if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == on && x_key_flag == off)
                            {

                                trans_key_data = "S";

                            }

                            button_W.Enabled = true;

                        }

                        else if (e.KeyCode == Keys.A) // When A Key is Pulled.
                        {

                            a_key_flag = off;


                            if (w_key_flag == on && a_key_flag == off &&

                                       s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {

                                trans_key_data = "W";

                            }

                            if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == off && x_key_flag == on)
                            {

                                trans_key_data = "X";

                            }

                            button_A.Enabled = true;

                        }
                        else if (e.KeyCode == Keys.S)
                        {

                            s_key_flag = off;

                            button_S.Enabled = true;


                        }
                        else if (e.KeyCode == Keys.D)
                        {

                            d_key_flag = off;

                            if (w_key_flag == on && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {

                                trans_key_data = "W";

                            }

                            if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == off && x_key_flag == on)
                            {

                                trans_key_data = "X";

                            }

                            button_D.Enabled = true;

                        }
                        else if (e.KeyCode == Keys.X)
                        {

                            x_key_flag = off;

                            if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {

                                trans_key_data = "S";

                            }

                            if (w_key_flag == off && a_key_flag == on &&

                          s_key_flag == off && d_key_flag == off && x_key_flag == off)
                            {


                                trans_key_data = "S";
                                //    textBox_KeyDir.Text = trans_key_data;

                            }

                            if (w_key_flag == off && a_key_flag == off &&

                          s_key_flag == off && d_key_flag == on && x_key_flag == off)
                            {

                                trans_key_data = "S";
                                //   textBox_KeyDir.Text = trans_key_data;

                            }

                            button_X.Enabled = true;

                        }

                        // LED BUTTON //
                        else if (e.KeyCode == Keys.H)
                        {

                            button_H.Enabled = true;


                        }

                        else if (e.KeyCode == Keys.J)
                        {

                            button_J.Enabled = true;

                        }

                        else if (e.KeyCode == Keys.K)
                        {

                            button_K.Enabled = true;

                        }

                        else if (e.KeyCode == Keys.L)
                        {

                            button_L.Enabled = true;

                        }

                    }

                            if (checkBox_Power_Controller_Activate.Checked)
                            {
                                    if (e.KeyCode == Keys.N)
                                    {


                                        button_N.Enabled = true;


                                    }
                                    else if (e.KeyCode == Keys.M)
                                    {

                                        button_M.Enabled = true;



                                    }
                            }

        }


        private void timer1_Tick(object sender, EventArgs e)
        {

            if (trans_key_data != "")
            {

                if (trans_key_data == "W")
                {

                    if (trans_key_w_state == off)
                    {


                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Forward " + "\n";

                        trans_key_w_state = on;
                        trans_key_x_state = off;
                        trans_key_s_state = off;
                        trans_key_q_state = off;
                        trans_key_e_state = off;
                        trans_key_z_state = off;
                        trans_key_c_state = off;



                    }


                }
                else if (trans_key_data == "S")
                {


                    if (trans_key_s_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Hold" + "\n";


                        trans_key_w_state = off;
                        trans_key_x_state = off;
                        trans_key_s_state = on;
                        trans_key_q_state = off;
                        trans_key_e_state = off;
                        trans_key_z_state = off;
                        trans_key_c_state = off;


                    }



                }
                else if (trans_key_data == "X")
                {

                    if (trans_key_x_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Backward" + "\n";

                        trans_key_w_state = off;
                        trans_key_x_state = on;
                        trans_key_s_state = off;
                        trans_key_q_state = off;
                        trans_key_e_state = off;
                        trans_key_z_state = off;
                        trans_key_c_state = off;

                    }




                }
                else if (trans_key_data == "Q")
                {

                    if (trans_key_q_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Forward & Left" + "\n";


                        trans_key_w_state = off;
                        trans_key_x_state = off;
                        trans_key_s_state = off;
                        trans_key_q_state = on;
                        trans_key_e_state = off;
                        trans_key_z_state = off;
                        trans_key_c_state = off;


                    }




                }
                else if (trans_key_data == "E")
                {

                    if (trans_key_e_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Forward & Right" + "\n";


                        trans_key_w_state = off;
                        trans_key_x_state = off;
                        trans_key_s_state = off;
                        trans_key_q_state = off;
                        trans_key_e_state = on;
                        trans_key_z_state = off;
                        trans_key_c_state = off;


                    }



                }
                else if (trans_key_data == "Z")
                {

                    if (trans_key_z_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);
                        richTextBox_Console.Text += "Car Direction : Forward & Left" + "\n";


                        trans_key_w_state = off;
                        trans_key_x_state = off;
                        trans_key_s_state = off;
                        trans_key_q_state = off;
                        trans_key_e_state = off;
                        trans_key_z_state = on;
                        trans_key_c_state = off;


                    }



                }
                else if (trans_key_data == "C")
                {

                    if (trans_key_c_state == off)
                    {

                        if (serialPort1.IsOpen) serialPort1.WriteLine(trans_key_data);

                        richTextBox_Console.Text += "Car Direction : Backward & Right" + "\n";


                        trans_key_w_state = off;
                        trans_key_x_state = off;
                        trans_key_s_state = off;
                        trans_key_q_state = off;
                        trans_key_e_state = off;
                        trans_key_z_state = off;
                        trans_key_c_state = on;


                    }



                }


            }

        }

        private void checkBox_Car_Controller_Actiavte_CheckedChanged(object sender, EventArgs e)
        {


            if (checkBox_Car_Controller_Actiavte.Checked)
            {


                richTextBox_Console.Text += "Car Controller : ON\n";
                if (serialPort1.IsOpen)
                {

                    serialPort1.Write("Key Mode = ON\n");




                }


            }

           else if (!checkBox_Car_Controller_Actiavte.Checked)
            {


                richTextBox_Console.Text += "Car Controller : OFF\n";
                if (serialPort1.IsOpen)
                {
                    serialPort1.Write("Key Mode = OFF\n");


                }


            }

        }

        private void button_open_Click(object sender, EventArgs e)
        {


            try
            {

                serialPort1.PortName = comboBox_com.Text;
                serialPort1.BaudRate = Convert.ToInt32(comboBox_baud.Text);
                serialPort1.DataBits = 8;
                serialPort1.StopBits = (StopBits)1;
                serialPort1.Parity = (Parity)0;


                serialPort1.Open();
                progressBar1.Value = 100;
                richTextBox_Console.Text += "Serial Port is Open...!!!\n";

            }
            catch (Exception err)
            {


                MessageBox.Show(err.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }


        }

        private void button_close_Click(object sender, EventArgs e)
        {



            if (serialPort1.IsOpen)
            {

                serialPort1.Close();
                progressBar1.Value = 0;
                comboBox_com.Items.Clear();
                comboBox_baud.Items.Clear();
                this.comboBox_baud.Items.AddRange(new object[] {
            "9600",
            "38400",
            "115200"});
                richTextBox_Console.Text += "Serial Port is Close...!!!\n";


            }


        }

        private void checkBox_Power_Controller_Activate_CheckedChanged(object sender, EventArgs e)
        {


            if (checkBox_Power_Controller_Activate.Checked)
            {


                richTextBox_Console.Text += "--- Dial Mode : OFF ---\n";

                trackBar_m.Enabled = true;
                trackBar_m1.Enabled = true;
                trackBar_m2.Enabled = true;
                trackBar_m3.Enabled = true;
                trackBar_m4.Enabled = true;
                button_N.Enabled = true;
                button_M.Enabled = true;

                if (serialPort1.IsOpen)
                {

                    serialPort1.Write("Dial Mode = OFF\n");

                }

            }
           else   if (!checkBox_Power_Controller_Activate.Checked )
            {

                richTextBox_Console.Text += "--- Dial Mode : ON ---\n";

                trackBar_m.Enabled = false;
                trackBar_m1.Enabled = false;
                trackBar_m2.Enabled = false;
                trackBar_m3.Enabled = false;
                trackBar_m4.Enabled = false;
                button_N.Enabled = false;
                button_M.Enabled = false;


                if (serialPort1.IsOpen)
                {

                    serialPort1.Write("Dial Mode = ON\n");


                }


            }


        }

        private void trackBar_m1_Scroll(object sender, EventArgs e)
        {

            Delay(100);

            trackbar_m1  = Convert.ToString(trackBar_m1.Value);
             label_m1.Text = trackbar_m1;

            if (serialPort1.IsOpen)
            {

                trackbar_m1 = "1:" + trackbar_m1 + "\n";
                serialPort1.Write(trackbar_m1);

            }
            Delay(100);

        }

        private void trackBar_m2_Scroll(object sender, EventArgs e)
        {

            Delay(100);
            trackbar_m2 = Convert.ToString(trackBar_m2.Value);
            label_m2.Text = trackbar_m2;

            if (serialPort1.IsOpen)
            {

                trackbar_m2 = "2:" + trackbar_m2 + "\n";
                serialPort1.Write(trackbar_m2);


            }
            Delay(100);


        }

        private void trackBar_m3_Scroll(object sender, EventArgs e)
        {
            Delay(100);
            trackbar_m3 = Convert.ToString(trackBar_m3.Value);
            label_m3.Text = trackbar_m3;


            if (serialPort1.IsOpen)
            {

                trackbar_m3 = "3:" + trackbar_m3 + "\n";
         
                serialPort1.Write(trackbar_m3);



            }
            Delay(100);


        }

        private void trackBar_m4_Scroll(object sender, EventArgs e)
        {
            Delay(100);

            trackbar_m4 = Convert.ToString(trackBar_m4.Value);
            label_m4.Text = trackbar_m4;


     
            if (serialPort1.IsOpen)
            {

                trackbar_m4 = "4:" + trackbar_m4 + "\n";
            
                serialPort1.Write(trackbar_m4);



            }
            Delay(100);


        }

        private void trackBar_m_Scroll(object sender, EventArgs e)
        {
           
            
            Delay(100);
            trackbar_m = Convert.ToString(trackBar_m.Value);
            label_m.Text = trackbar_m;


            trackBar_m1.Value = trackBar_m.Value;
            label_m1.Text = trackbar_m;
            trackBar_m2.Value = trackBar_m.Value;
            label_m2.Text = trackbar_m;
            trackBar_m3.Value = trackBar_m.Value;
            label_m3.Text = trackbar_m;
            trackBar_m4.Value = trackBar_m.Value;
            label_m4.Text = trackbar_m;


            if (serialPort1.IsOpen)
            {

                trackbar_m = "5:" + trackbar_m + "\n";
                serialPort1.Write(trackbar_m);


            }
            Delay(100);


        }

        private void comboBox_com_Click(object sender, EventArgs e)
        {


            comboBox_com.Items.Clear();
            string[] PORT_NAME = SerialPort.GetPortNames();

            comboBox_com.Items.AddRange(PORT_NAME);


        }
    }
}
