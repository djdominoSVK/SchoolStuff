using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace AddressBook
{
    public partial class Edit : Form
    {

        private int idLoaded = -1;

        private DataSet1.recordDataRow recordRow;

        public DataSet1.recordDataRow RecordRow
        {
            get
            {
                set_recordRow(recordRow);
                return recordRow;
            }
            set
            {
                recordRow = value;
            }
        }



        public Edit()
        {
            InitializeComponent();
            RecordRow = dataSet1.recordData.NewrecordDataRow();
            pictureBox1.Load("no_photo.jpg");
            pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            Color fore_color;
            fore_color = Color.FromKnownColor(KnownColor.InactiveCaptionText);
            foreach (Control ctl in groupBox2.Controls)
            {
                ctl.ForeColor = fore_color;
            }
            this.relationBox.SelectedIndex = 0;
            this.num1.SelectedIndex = 0;
            this.num2.SelectedIndex = 0;
            this.num3.SelectedIndex = 0;
            this.num4.SelectedIndex = 0;
            this.num5.SelectedIndex = 0;
            this.num6.SelectedIndex = 0;
        }

        public Edit(DataSet1.recordDataRow row)
        {
            InitializeComponent();
            idLoaded = row.id;
            RecordRow = dataSet1.recordData.NewrecordDataRow();
            this.relationBox.Text=row.relation;
            this.titleText.Text=row.title;
            this.fnameText.Text=row.first_name;
            this.midnameText.Text=row.mid_name;
            this.lnameText.Text=row.last_name;
            this.nickText.Text=row.nickname;
            if(row.sex == "Male"){male.Checked=true;}
            else { female.Checked = true; };
            if (row.birthdate != null)
            {
                this.dateTimePicker1.Checked = true;
                this.dateTimePicker1.Value = System.DateTime.Parse(row.birthdate);
            }
            this.noteText.Text=row.note;
            this.pictureBox1.ImageLocation=row.photo;
            this.pictureBox1.Image= new Bitmap(row.photo);
            this.pictureBox1.SizeMode=PictureBoxSizeMode.StretchImage;
            this.pcityText.Text=row.pcity;
            this.paddressText.Text=row.paddress;
            this.pzipText.Text=row.pzip;
            if (((row.scity.Length>0) || (row.sddress.Length>0))
                || ((row.szip.Length > 0) || (row.sstate.Length > 0)))
            {
                this.checkGroupBox2.Checked = true;
                this.groupBox2.Enabled = true;
                this.pstateText.Text = row.pstate;
                this.scityText.Text = row.scity;
                this.saddText.Text = row.sddress;
                this.szipText.Text = row.szip;
                this.sstateText.Text = row.sstate;
            }
            this.num1.Text=row.num1;
            this.num2.Text=row.num2;
            this.num3.Text=row.num3;
            this.num4.Text=row.num4;
            this.num5.Text=row.num5;
            this.num6.Text=row.num6;
            this.num1Text.Text=row.nnum1;
            this.num2Text.Text=row.nnum2;
            this.num3Text.Text=row.nnum3;
            this.num4Text.Text=row.nnum4;
            this.num5Text.Text=row.nnum5;
            this.num6Text.Text=row.nnum6;
            this.emailText.Text=row.email;
            this.webText.Text=row.web;
            this.wpositionText.Text=row.wposition;
            this.wcopmanyText.Text=row.wcompany;
            this.waddressText.Text=row.waddress;
            this.wcityText.Text=row.wcity;
            this.wzipText.Text=row.wzip;
            this.wtelText.Text=row.wtelephone;
            this.wemailText.Text=row.wemail;
            this.wwebText.Text = row.wweb;
        }



        private void cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void save_Click(object sender, EventArgs e)
        {
 
            if (ValidateChildren())
            {
                DialogResult = DialogResult.OK;
            }
            else
            {
                DialogResult = DialogResult.None;
                MessageBox.Show("You have entered invalid data.");
            }
  
        }


        private void set_recordRow(DataSet1.recordDataRow record)
        {        
            string gender = male.Checked ? "Male" : "Female";
            record.id = idLoaded;
            record.relation = this.relationBox.Text;
            record.title = this.titleText.Text;
            record.first_name = this.fnameText.Text;
            record.mid_name = this.midnameText.Text;
            record.last_name = this.lnameText.Text;
            record.nickname = this.nickText.Text;
            record.sex = gender;
            if (this.dateTimePicker1.Checked)
            {
                record.birthdate = this.dateTimePicker1.Text;
            }
            record.note = this.noteText.Text;
            record.photo = this.pictureBox1.ImageLocation;
            record.pcity = this.pcityText.Text;
            record.paddress = this.paddressText.Text;
            record.pzip = this.pzipText.Text;
            record.pstate = this.pstateText.Text;
            record.scity = this.scityText.Text;
            record.sddress = this.saddText.Text;
            record.szip = this.szipText.Text;
            record.sstate = this.sstateText.Text;
            record.num1 = this.num1.Text;
            record.num2 = this.num2.Text;
            record.num3 = this.num3.Text;
            record.num4 = this.num4.Text;
            record.num5 = this.num5.Text;
            record.num6 = this.num6.Text;
            record.nnum1 = this.num1Text.Text;
            record.nnum2 = this.num2Text.Text;
            record.nnum3 = this.num3Text.Text;
            record.nnum4 = this.num4Text.Text;
            record.nnum5 = this.num5Text.Text;
            record.nnum6 = this.num6Text.Text;
            record.email = this.emailText.Text;
            record.web = this.webText.Text;
            record.wposition = this.wpositionText.Text;
            record.wcompany = this.wcopmanyText.Text;
            record.waddress = this.waddressText.Text;
            record.wcity = this.wcityText.Text;
            record.wzip = this.wzipText.Text;
            record.wtelephone = this.wtelText.Text;
            record.wemail = wemailText.Text;
            record.wweb = wwebText.Text;
        }


        private void picture_Click(object sender, EventArgs e)
        {
            try
            {
                OpenFileDialog open = new OpenFileDialog();
                open.Filter = "Image Files(*.jpg; *.jpeg; *.gif; *.bmp)|*.jpg; *.jpeg; *.gif; *.bmp";
                if (open.ShowDialog() == DialogResult.OK)
                {
                    pictureBox1.Image = new Bitmap(open.FileName);
                    pictureBox1.ImageLocation = open.FileName;
                    pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
                }
            }
            catch (Exception)
            {
                throw new ApplicationException("Failed loading image");
            }
        }

        private void fnameText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !fnameText.Text.All(Char.IsLetter) ||
                string.IsNullOrWhiteSpace(fnameText.Text);
            if (e.Cancel)
            {
                errorProvider1.SetError(fnameText, "You have to enter valid first name.");
            }
            else
            {
                errorProvider1.SetError(fnameText, "");
                errorProvider1.SetError(fnameText, "");
            }
        }

        private void lnameText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !lnameText.Text.All(Char.IsLetter) || string.IsNullOrWhiteSpace(lnameText.Text);
            if (e.Cancel)
            {

                errorProvider2.SetError(lnameText, "You have to enter valid last name.");
            }
            else
            {
                errorProvider2.SetError(lnameText, "");
            }
        }

        private void checkGroupBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkGroupBox2.Parent == groupBox2)
            {
                groupBox2.Parent.Controls.Add(checkGroupBox2);

                checkGroupBox2.Location = new Point(
                    checkGroupBox2.Left + groupBox2.Left,
                    checkGroupBox2.Top + groupBox2.Top);

                checkGroupBox2.BringToFront();
            }
            groupBox2.Enabled = (checkGroupBox2.Checked);

            Color fore_color;
            if (groupBox2.Enabled)
            {
                fore_color = Color.FromKnownColor(KnownColor.ActiveCaptionText);
            }
            else
            {
                fore_color = Color.FromKnownColor(KnownColor.InactiveCaptionText);
            }
            foreach (Control ctl in groupBox2.Controls)
            {
                ctl.ForeColor = fore_color;
            }
        }


        private void num1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num1.SelectedIndex != 0)
            {
                num1Text.Enabled = true;
            }
            else {
                num1Text.Enabled = false;
            }
        }

        private void num2_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num2.SelectedIndex != 0)
            {
                num2Text.Enabled = true;
            }
            else
            {
                num2Text.Enabled = false;
            }
        }

        private void num3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num3.SelectedIndex != 0)
            {
                num3Text.Enabled = true;
            }
            else
            {
                num3Text.Enabled = false;
            }
        }

        private void num4_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num4.SelectedIndex != 0)
            {
                num4Text.Enabled = true;
            }
            else
            {
                num4Text.Enabled = false;
            }
        }

        private void num5_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num5.SelectedIndex != 0)
            {
                num5Text.Enabled = true;
            }
            else
            {
                num5Text.Enabled = false;
            }
        }

        private void num6_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.num6.SelectedIndex != 0)
            {
                num6Text.Enabled = true;
            }
            else
            {
                num6Text.Enabled = false;
            }
        }

        private void pzipText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !pzipText.Text.All(Char.IsDigit) && !string.IsNullOrEmpty(pzipText.Text);
            if (e.Cancel)
            {

                errorProvider3.SetError(pzipText, "You have to enter valid ZIP code.");
            }
            else
            {
                errorProvider3.SetError(pzipText, "");
            }
        }

        private void szipText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !szipText.Text.All(Char.IsDigit) && !string.IsNullOrEmpty(szipText.Text);
            if (e.Cancel)
            {

                errorProvider4.SetError(szipText, "You have to enter valid ZIP code.");
            }
            else
            {
                errorProvider4.SetError(szipText, "");
            }
        }

        private void emailText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !(((emailText.Text.IndexOf('@') > 0) && (emailText.Text.LastIndexOf('@') < (emailText.Text.Length-1))
               && (emailText.Text.IndexOf('@') == emailText.Text.LastIndexOf('@'))) || (string.IsNullOrEmpty(emailText.Text)));
            if (e.Cancel)
            {

                errorProvider5.SetError(emailText, "You have to enter valid email address.");
            }
            else
            {
                errorProvider5.SetError(emailText, "");

            }
        }

        private void wzipText_Validating(object sender, CancelEventArgs e)
        {

            e.Cancel = !wzipText.Text.All(Char.IsDigit) && !string.IsNullOrEmpty(wzipText.Text);
            if (e.Cancel)
            {

                errorProvider6.SetError(wzipText, "You have to enter valid ZIP code.");
            }
            else
            {
                errorProvider6.SetError(wzipText, "");
            }
        }

        private void wemailText_Validating(object sender, CancelEventArgs e)
        {
            e.Cancel = !(((wemailText.Text.IndexOf('@') > 0) && (wemailText.Text.LastIndexOf('@') < (wemailText.Text.Length - 1))
                        && (wemailText.Text.IndexOf('@') == wemailText.Text.LastIndexOf('@')))|| (string.IsNullOrEmpty(wemailText.Text)));
            if (e.Cancel)
            {

                errorProvider7.SetError(wemailText, "You have to enter valid email address.");
            }
            else
            {
                errorProvider7.SetError(wemailText, "");

            }
        }

    }
}
