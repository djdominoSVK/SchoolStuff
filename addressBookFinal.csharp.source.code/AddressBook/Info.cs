using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AddressBook
{
    public partial class Info : Form
    {
        private bool edited;

        public bool Edited
        {
            get { return edited; }
            set { edited = value; }
        }

        private DataSet1.recordDataRow loadedRow;

        public DataSet1.recordDataRow LoadedRow
        {
            get { return loadedRow; }
            set { loadedRow = value; }
        }

        public Info()
        {
            InitializeComponent();
        }

        public Info(DataSet1.recordDataRow row)
        {
            InitializeComponent();
            this.Text = row.title + ' ' + row.first_name + ' ' + row.mid_name + ' ' + row.last_name;
            this.relationText.Text = row.relation;
            this.nameText.Text = this.Text;
            this.nickText.Text = row.nickname;
            this.sexText.Text = row.sex;
            this.birthdateText.Text = row.birthdate;
            this.noteText.Text = row.note;
            this.photoPictureBox.Image = new Bitmap(row.photo);
            this.photoPictureBox.ImageLocation = row.photo;
            this.photoPictureBox.SizeMode=PictureBoxSizeMode.StretchImage;
            this.pcityText.Text = row.pcity;
            this.paddressText.Text = row.paddress;
            this.pzipText.Text = row.pzip;
            this.pstateText.Text = row.pstate;
            this.scityText.Text = row.scity;
            this.saddText.Text = row.sddress;
            this.szipText.Text = row.szip;
            this.sstateText.Text = row.sstate;
            this.num1.Text = row.num1;
            this.num2.Text = row.num2;
            this.num3.Text = row.num3;
            this.num4.Text = row.num4;
            this.num5.Text = row.num5;
            this.num6.Text = row.num6;
            this.nnum1.Text = row.nnum1;
            this.nnum2.Text = row.nnum2;
            this.nnum3.Text = row.nnum3;
            this.nnum4.Text = row.nnum4;
            this.nnum5.Text = row.nnum5;
            this.nnum6.Text = row.nnum6;
            this.emailText.Text = row.email;
            this.webText.Text = row.web;
            this.wpositionText.Text = row.wposition;
            this.companyText.Text = row.wcompany;
            this.waddressText.Text = row.waddress;
            this.wcityText.Text = row.wcity;
            this.wzipText.Text = row.wzip;
            this.wtelText.Text = row.wtelephone;
            this.wemailText.Text = row.wemail;
            this.wwebText.Text = row.wweb;
            loadedRow = row;
        }

        private void editContactbutton1_Click(object sender, EventArgs e)
        {
            Edit dlg = new Edit(this.loadedRow);
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                this.loadedRow = dlg.RecordRow;
                edited = true;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
