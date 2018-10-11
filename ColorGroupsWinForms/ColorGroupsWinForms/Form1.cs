using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Data.Linq;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.IO;
using ColorGroupsWrapper;
using System.Drawing.Imaging;

namespace ColorGroupsWinForms
{
    public partial class Form1 : Form
    {
        Table<ImageTable> ImageTables;
        ImageDBDataContext db;
        int indexId = 0;
        IntPtr srcImagePtr;
        string filePath;
        string fileName;

        int[] unknownsArr;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'sqlDbDataSet1.Table' table. You can move, or remove it, as needed.
            //this.tableTableAdapter.Fill(this.sqlDbDataSet1.Table);
            // TODO: This line of code loads data into the 'sqlDbDataSet.Table' table. You can move, or remove it, as needed.
            this.tableTableAdapter.Fill(this.sqlDbDataSet.Table);







        }

        private void button1_Click(object sender, EventArgs e)
        {
            

            OpenFileDialog file = new OpenFileDialog();
            file.RestoreDirectory = true;
            file.CheckFileExists = false;
            file.Filter = "Image Files(JPG, PNG)| *.JPG; *.PNG";

            if (file.ShowDialog() != DialogResult.OK) return;


            filePath = file.FileName;
            fileName = file.SafeFileName;

            Bitmap srcBitmap = new Bitmap(filePath);
           
            int w = srcBitmap.Width;
            int h = srcBitmap.Height;
            Rectangle rect = new Rectangle(0, 0, w, h);
            
            
            BitmapData bmpData = srcBitmap.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadWrite,System.Drawing.Imaging.PixelFormat.Format24bppRgb);

            IntPtr ptr = bmpData.Scan0;
            srcImagePtr = ColorGrps.CreateColorGroupsWithBitmap(ptr, w, h);

            srcBitmap.UnlockBits(bmpData);

            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox1.Image = srcBitmap;
            textBox1.Text = filePath;

           

            //for opencv string structure
            /*pictureBox1.ImageLocation = filePath;
            
            if(filePath != null) {
                string[] temp = filePath.Split('\\');
                filePath = "";
                foreach ( string s in temp)
                {
                    filePath += s;
                    filePath += "/";
                }
                filePath = filePath.Remove(filePath.Length - 1);
            }*/

            // srcImagePtr =  ColorGrps.CreateColorGroups(filePath);

            IntPtr unknowns = ColorGrps.GetUnknownCount(srcImagePtr);
            unknownsArr = new int[20];

            Marshal.Copy(unknowns, unknownsArr,0,20);

            if (srcImagePtr != IntPtr.Zero)
            {
                comboBox1.Items.Clear();
                if (ColorGrps.GetBlueCount(srcImagePtr) != 0) comboBox1.Items.Add("Blue");
                if (ColorGrps.GetRedCount(srcImagePtr) != 0) comboBox1.Items.Add("Red");
                if (ColorGrps.GetGreenCount(srcImagePtr) != 0) comboBox1.Items.Add("Green");
                if (ColorGrps.GetYellowCount(srcImagePtr) != 0) comboBox1.Items.Add("Yellow");
                if (ColorGrps.GetPinkCount(srcImagePtr) != 0) comboBox1.Items.Add("Pink");
                if (ColorGrps.GetOrangeCount(srcImagePtr) != 0) comboBox1.Items.Add("Orange");
                if (unknownsArr[0] != -1) comboBox1.Items.Add("Unknown Colors");
            }


            DateTime dt = DateTime.Now ;
            String groupCounts = "Black="+ColorGrps.GetBlackCount(srcImagePtr).ToString() + ";" + "Blue=" + ColorGrps.GetBlueCount(srcImagePtr).ToString() + ";";
            groupCounts += "Red="+ColorGrps.GetRedCount(srcImagePtr).ToString() + ";" + "Green=" + ColorGrps.GetGreenCount(srcImagePtr).ToString() + ";";
            groupCounts += "Yellow="+ColorGrps.GetYellowCount(srcImagePtr).ToString() + ";" + "Pink=" + ColorGrps.GetPinkCount(srcImagePtr).ToString() + ";";
            groupCounts += "Orange=" + ColorGrps.GetOrangeCount(srcImagePtr).ToString() + ";" + "Unknowns=";
            if (unknownsArr[0] == -1) groupCounts += "0";
            
            else {
                int temp;
                for (temp = 0; unknownsArr[temp + 1] != -1; temp++)
                    groupCounts += unknownsArr[temp].ToString() + ",";
                groupCounts += unknownsArr[temp].ToString();

            }
            
            byte[] byteArray = new byte[0];
            using (MemoryStream stream = new MemoryStream())
            {
                srcBitmap.Save(stream, System.Drawing.Imaging.ImageFormat.Png);
                stream.Close();

                byteArray = stream.ToArray();
            }

            db = new ImageDBDataContext();
            Binary binary = new Binary(byteArray);


            
            var imgTable = new ImageTable
            {
               
                ImageName =  fileName,
                Image = binary,
                ColorCount = ColorGrps.GetGroupCount(srcImagePtr),
                ColorGroupCounts = groupCounts,
                Date = dt

            };


            Table<ImageTable> imgTables = db.ImageTables;

            db.ImageTables.InsertOnSubmit(imgTable);
            db.SubmitChanges();
            
            loadData();




        }

        private void loadData()
        {
            db = new ImageDBDataContext();
            dataGridView1.DataSource = db.ImageTables;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {

        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int width = ColorGrps.GetWidth(srcImagePtr);
            int height = ColorGrps.GetHeight(srcImagePtr);
            pictureBox2.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox2.Image = null;
            if (srcImagePtr != IntPtr.Zero )
            {
                if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Blue" && ColorGrps.GetBlueCount(srcImagePtr) != 0)
                {
                    IntPtr imgPtr = ColorGrps.ShowBlueColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetBlueCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Black" && ColorGrps.GetBlackCount(srcImagePtr) != 0)
                {
                    
                    textBox2.Text = ColorGrps.GetBlackCount(srcImagePtr).ToString();
                }

                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Red" && ColorGrps.GetRedCount(srcImagePtr) != 0)
                {
                    IntPtr imgPtr = ColorGrps.ShowRedColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetRedCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Green" && ColorGrps.GetGreenCount(srcImagePtr) != 0)
                {
                    IntPtr imgPtr = ColorGrps.ShowGreenColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetGreenCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Yellow" && ColorGrps.GetYellowCount(srcImagePtr) != 0)
                {
                    IntPtr imgPtr = ColorGrps.ShowYellowColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetYellowCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Pink" && ColorGrps.GetPinkCount(srcImagePtr) != 0)
                {

                    IntPtr imgPtr = ColorGrps.ShowPinkColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetPinkCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Orange" && ColorGrps.GetOrangeCount(srcImagePtr) != 0)
                {
                    IntPtr imgPtr = ColorGrps.ShowOrangeColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;
                    textBox2.Text = ColorGrps.GetOrangeCount(srcImagePtr).ToString();
                }
                else if (this.comboBox1.GetItemText(this.comboBox1.SelectedItem) == "Unknown Colors" && unknownsArr[0] != -1)
                {
                    
                    IntPtr imgPtr = ColorGrps.ShowUnknownColorGroups(srcImagePtr);
                    Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
                    BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    Bitmap a = new Bitmap(width, height, bmpData.Stride, PixelFormat.Format24bppRgb, imgPtr);
                    pictureBox2.Image = a;

                    textBox2.Text = "";
                    int i;
                    for(i =0; unknownsArr[i+1]!= -1; i++)
                        textBox2.Text += unknownsArr[i].ToString() +";";
                    textBox2.Text += unknownsArr[i].ToString();
                }
            }


        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }
        
        private void dataGridView1_CellContentClick_1(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
