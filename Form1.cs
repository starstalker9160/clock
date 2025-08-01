using System;
using System.IO;
using System.Drawing;
using System.Reflection;
using System.Drawing.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace clock {
    public partial class Form1 : Form {

        private bool isFullScreen = false;
        private FormWindowState previousWindowState;
        private FormBorderStyle previousBorderStyle;
        private Rectangle previousBounds;
        private Label label1;

        private PrivateFontCollection fonts = new PrivateFontCollection();

        public Form1() {
            InitializeComponent();

            string resourceName = "clock.Properties.Rodin.otf";
            using (Stream fontStream = Assembly.GetExecutingAssembly().GetManifestResourceStream(resourceName)) {
                if (fontStream == null) {
                    MessageBox.Show("Font resource not found");
                    return;
                }

                byte[] fontData = new byte[fontStream.Length];
                fontStream.Read(fontData, 0, (int)fontStream.Length);

                IntPtr fontPtr = Marshal.AllocCoTaskMem(fontData.Length);
                Marshal.Copy(fontData, 0, fontPtr, fontData.Length);
                fonts.AddMemoryFont(fontPtr, fontData.Length);
                Marshal.FreeCoTaskMem(fontPtr);
            }

            label1 = new Label();
            label1.Font = new Font(fonts.Families[0], 90);
            label1.AutoSize = true;
            label1.ForeColor = ColorTranslator.FromHtml("#F6F9FB");
            Controls.Add(label1);

            timer1 = new Timer();
            timer1.Interval = 1000;
            timer1.Tick += timer1_Tick_1;
            timer1.Start();

            this.Text = "Clock";
            this.BackColor = ColorTranslator.FromHtml("#11101A");
            this.Resize += (s, e) => CenterLabel();

            CenterLabel();
        }

        private void timer1_Tick_1(object sender, EventArgs e) {
            label1.Text = DateTime.Now.ToString("HH:mm:ss");
        }

        private void CenterLabel() {
            if (label1 != null) {
                label1.Location = new Point(
                    (ClientSize.Width - label1.Width) / 2,
                    (ClientSize.Height - label1.Height) / 2
                );
            }
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData) {
            if (keyData == (Keys.Alt | Keys.Enter)) {
                ToggleFullScreen();
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void ToggleFullScreen() {
            if (!isFullScreen) {
                previousWindowState = this.WindowState;
                previousBorderStyle = this.FormBorderStyle;
                previousBounds = this.Bounds;

                this.FormBorderStyle = FormBorderStyle.None;
                this.WindowState = FormWindowState.Maximized;
                isFullScreen = true;
            } else {
                this.FormBorderStyle = previousBorderStyle;
                this.WindowState = previousWindowState;
                this.Bounds = previousBounds;
                isFullScreen = false;
            }

            CenterLabel();
        }
    }
}