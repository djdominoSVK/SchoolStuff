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
    public partial class MainMenu : Form
    {

        private int counter = 0;
        private string filename = null;
        private string photo_dir = null;
        private bool changed = false;

        public MainMenu()
        {
            InitializeComponent();
            FileInfo fi = new System.IO.FileInfo("file.log");
            if (fi.Exists && (fi.Length > 0)){
                openAddressBook(loadLogFile());
            }
            else{
                createEmptyAddressbook();
                ShowAdressBookName();
                ShowItems();
            }
            relationtoolStripComboBox1.SelectedIndex = 0;
            sortBytoolStripComboBox1.SelectedIndex = 0;
        }


        private void openAddressBookViaDialog()
        {
            try
            {
                OpenFileDialog dlg = new OpenFileDialog();
                dlg.Title = "Open address book";
                dlg.Filter = "xml files (*.xml)|*.xml";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    openAddressBook(dlg.FileName);
                }
            }
            catch (IOException f)
            {
                MessageBox.Show("Error while loading address book from file:\r\n" + f);
            }
        }

        private void openAddressBook(string file)
        {
            try
            {
                FileInfo fi = new System.IO.FileInfo(file);

                this.dataSet1.recordData.Clear();
                if (fi.Exists)
                {
                    if (fi.Length > 0)
                    {
                        this.dataSet1.ReadXml(file);                   
                    }
                    filename = file;
                    if (Path.GetDirectoryName(file) == "")
                    {
                        photo_dir = Path.GetDirectoryName(file) + Path.GetFileNameWithoutExtension(filename);
                    }
                    else {
                        photo_dir = Path.GetDirectoryName(file) + "\\" + Path.GetFileNameWithoutExtension(filename);   
                    }
                    counter = this.dataSet1.recordData.Rows.Count;
                    updateListView(sort(filter()));
                    this.changed = false;
                    ShowAdressBookName();
                    ShowItems();
                    saveLogFile();
                }
                else {
                    createEmptyAddressbook();
                    ShowAdressBookName();
                    ShowItems();
                }
            
            }
            catch (IOException f)
            {
                MessageBox.Show("Error while loading address book from file:\r\n" + f);
            }
        }

        private void NewAdressBook(string file)
        {
            try
            {
                this.dataSet1.recordData.Clear();
                filename = file;
                photo_dir = Path.GetDirectoryName(file) + Path.GetFileNameWithoutExtension(filename);
                Directory.CreateDirectory(photo_dir);
                File.Create(filename);
                updateListView(this.dataSet1.recordData);
                this.changed = false;
                ShowAdressBookName();
                ShowItems();
                saveLogFile();

            }
            catch (IOException e)
            {
                MessageBox.Show("Error while creating new address book to file:\r\n" + e);
            }
        }


        private void NewAdressBookDialog()
        {
            try
            {
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.Title = "Choose name for new address book";
                dlg.Filter = "xml files (*.xml)|*.xml";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    filename = dlg.FileName;
                    photo_dir = Path.GetDirectoryName(dlg.FileName) + Path.GetFileNameWithoutExtension(filename);
                    if (Directory.Exists(photo_dir) && (Directory.GetFiles(photo_dir).Length > 0))
                    {
                        DialogResult msg = MessageBox.Show("Directory for photos already exist. Some files could be rewritten. Do you want to continue?",
                            "Directory for photos already exist", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                        if (msg == DialogResult.Yes)
                        {
                            NewAdressBook(dlg.FileName);
                        }
                        else { openAddressBookViaDialog(); }
                    }
                    else
                    {
                        NewAdressBook(dlg.FileName);
                    }
                }
            }


            catch (IOException e)
            {
                MessageBox.Show("Error while creating new address book to file:\r\n" + e);
            }
        }



        private void updateListView(DataSet1.recordDataDataTable table)
        {
            listView1.Clear();
            imageList1.Images.Clear();
            DirectoryInfo dir = new DirectoryInfo(photo_dir);
            if (dir.Exists)
            {
                for (int i = 0; i < table.Rows.Count; i++)
                {
                    this.imageList1.Images.Add(Image.FromFile((string)table.Rows[i]["photo"]));
                    this.imageList1.Images.SetKeyName(i, (string)table.Rows[i]["photo"]);
                }
                this.listView1.View = View.LargeIcon;
                this.imageList1.ImageSize = new Size(90, 115);
                this.listView1.LargeImageList = this.imageList1;

                for (int j = 0; j < table.Rows.Count; j++)
                {
                    ListViewItem item = new ListViewItem();
                    item.ImageKey = (string)table.Rows[j]["photo"];
                    item.Tag = (int)table.Rows[j]["id"];
                    item.Text = (string)table.Rows[j]["first_name"] + " " + (string)table.Rows[j]["last_name"];
                    this.listView1.Items.Add(item);
                }
                this.editContacttoolStripButton1.Enabled = this.editContactToolStripMenuItem.Enabled =
                    this.removeContacttoolStripButton1.Enabled = this.removeContactToolStripMenuItem.Enabled = false;
            }
            else{
                createEmptyAddressbook();
                ShowAdressBookName();
                ShowItems();
            }
        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {
            int index = (int)listView1.SelectedItems[0].Tag;
            DataSet1.recordDataRow newRow = dataSet1.recordData.FindByid(index);
            Info dlg = new Info(newRow);
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                if (dlg.Edited)
                {
                    dataSet1.recordData.FindByid(index).ItemArray = dlg.LoadedRow.ItemArray;
                    updateListView(sort(filter()));
                    this.changed = true;
                    ShowItems();
                }
            }
        }

        private void editContact()
        {
            int index = (int)listView1.SelectedItems[0].Tag;
            DataSet1.recordDataRow newRow = dataSet1.recordData.FindByid(index);
            Edit dlg = new Edit(newRow);
            dlg.Text = "Edit Contact";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                dataSet1.recordData.FindByid(index).ItemArray = dlg.RecordRow.ItemArray;
                if (dataSet1.recordData.FindByid(index).photo != "no_photo.jpg")
                {
                    string imagepath = dlg.RecordRow.photo;
                    string newPath = photo_dir + string.Format("\\photo{0}" + System.IO.Path.GetExtension(imagepath), dataSet1.recordData.FindByid(index).id);
                    File.Copy(imagepath, newPath, true);
                    dataSet1.recordData.FindByid(index).photo = newPath;
                }
                updateListView(sort(filter()));
                this.changed = true;
                ShowItems();
                SaveFile();
            }
        }

        private void deleteContact()
        {
            if (listView1.SelectedItems.Count == 1)
            {
                DialogResult dlg = MessageBox.Show("Do you really want to delete contact?", "Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (dlg == DialogResult.Yes)
                {
                    int index = (int)listView1.SelectedItems[0].Tag;
                    string fileToremove="";
                    if (dataSet1.recordData.FindByid(index).photo != "no_photo.jpg") 
                    {
                        fileToremove = dataSet1.recordData.FindByid(index).photo;
                    }
                    this.dataSet1.recordData.RemoverecordDataRow(dataSet1.recordData.FindByid(index));
                    this.changed = true;
                    this.counter--;
                    updateListView(sort(filter()));
                    ShowAdressBookName();
                    ShowItems();
                    SaveFile();
                    if (fileToremove != "") { File.Delete(fileToremove); }
                }
            }
        }

        private void newRecord()
        {
            Edit dlg = new Edit();
            dlg.Text = ("New Contact");
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                this.dataSet1.recordData.Rows.Add(dlg.RecordRow.ItemArray);
                if ((int)dataSet1.recordData.Rows[0]["id"] == -1)
                {
                    dataSet1.recordData.Rows[0]["id"] = 1;
                }
                else
                {
                    dataSet1.recordData.Rows[counter]["id"] = (int)dataSet1.recordData.Rows[counter - 1]["id"] + 1;
                }
                if (dlg.RecordRow.photo != "no_photo.jpg")
                {
                    string imagepath = dlg.RecordRow.photo;
                    string newPath = photo_dir + string.Format("\\photo{0}" + System.IO.Path.GetExtension(imagepath), dataSet1.recordData.Rows[counter]["id"]);
                    File.Copy(imagepath, newPath, true);
                    dataSet1.recordData.Rows[counter]["photo"] = newPath;
                }
                this.counter++;
                ShowAdressBookName();
                updateListView(sort(filter()));
                ShowAdressBookName();
                this.changed = true;
                ShowItems();
                contactIndexChanged();
                SaveFile();
            }
        }

        private void SaveFile()
        {
            try
            {
                this.dataSet1.recordData.WriteXml(filename);
                this.changed = false;
                ShowItems();
                saveLogFile();
            }
            catch (IOException e)
            {
                MessageBox.Show("Error while saving data to file:\r\n" + e);
            }
        }

        private void SaveFileAs()
        {
            try
            {
                SaveFileDialog dlg = new SaveFileDialog();
                dlg.Title = "Choose save file for address book";
                dlg.Filter = "xml files (*.xml)|*.xml";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    string old_filename = filename;
                    filename = dlg.FileName;
                    string old_dir = photo_dir;
                    photo_dir = Path.GetDirectoryName(dlg.FileName) + "\\" + Path.GetFileNameWithoutExtension(filename);
                    if (Directory.Exists(photo_dir) && (Directory.GetFiles(photo_dir).Length > 0))
                    {
                        DialogResult msg = MessageBox.Show("Directory for photos already exist. Some files could be rewritten. Do you want to continue?",
                            "Directory for photos already exist", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                        if (msg == DialogResult.Yes)
                        {
                            saveNewDir(photo_dir);
                        }
                        else
                        {
                            filename = old_filename;
                            photo_dir = old_dir;
                            SaveFileAs();
                        }
                    }
                    else
                    {
                        saveNewDir(photo_dir);
                    }

                }
            }

            catch (IOException e)
            {
                MessageBox.Show("Error while saving data to file:\r\n" + e);
            }
        }

        private void saveNewDir(string dir)
        {
            Directory.CreateDirectory(dir);
            for (int i = 0; i < this.dataSet1.recordData.Rows.Count; i++)
            {
                if ((string)this.dataSet1.recordData.Rows[i]["photo"] != "no_photo.jpg")
                {
                    string imagepath = (string)this.dataSet1.recordData.Rows[i]["photo"];
                    string newPath = dir + string.Format("\\\\photo{0}" + System.IO.Path.GetExtension(imagepath), this.dataSet1.recordData.Rows[i]["id"]);
                    File.Copy(imagepath, newPath, true);
                    dataSet1.recordData.Rows[i]["photo"] = newPath;
                }
            }
            this.changed = false;
            ShowAdressBookName();
            ShowItems();
            SaveFile();
        }

        private void contactIndexChanged()
        {
            this.editContacttoolStripButton1.Enabled = this.removeContacttoolStripButton1.Enabled =
            this.editContactToolStripMenuItem.Enabled = this.removeContactToolStripMenuItem.Enabled =
            (listView1.SelectedItems.Count == 1);
        }

        private void searchContacts(string text)
        {
            if (text != "")
            {
                DataSet1.recordDataDataTable newTable = new DataSet1.recordDataDataTable();
                DataRow[] drarray;
                int i = 0;
                int[] idArray = new int[this.dataSet1.recordData.Rows.Count];
                int idArrayCounter = 0;
                while (i < this.dataSet1.recordData.Columns.Count)
                {
                    if (this.dataSet1.recordData.Columns[i].DataType == typeof(string))
                    {
                        string expression = this.dataSet1.recordData.Columns[i].ColumnName + " LIKE '*" + text + "*'";
                        drarray = dataSet1.recordData.Select(expression);
                        for (int j = 0; j < drarray.Length; j++)
                        {
                            bool idExisting = false;
                            for (int k = 0; k < idArrayCounter; k++)
                            {
                                if ((int)drarray[j].ItemArray[0] == idArray[k]) { idExisting = true; }
                            }
                            if (!idExisting)
                            {
                                newTable.Rows.Add(drarray[j].ItemArray);
                                idArray[idArrayCounter] = (int)drarray[j].ItemArray[0];
                                idArrayCounter++;
                            }
                        }
                    }
                    i++;
                }
                updateListView(sort(newTable));
            }
        }


        private void ShowAdressBookName()
        {
            this.Text = string.Format("Adress Book - {0}", filename == null ? "no file loaded" : filename);
            AdressBookNametoolStripStatusLabel1.Text = filename == null ? "no file loaded" : filename;
            ContactCountertoolStripStatusLabel1.Text = string.Format("contacts: {0}", counter);
        }

        private void ShowItems()
        {
            this.saveasToolStripMenuItem.Enabled = this.contactToolStripMenuItem.Enabled =
                this.newContacttoolStripButton1.Enabled = this.relationtoolStripComboBox1.Enabled =
                this.sortBytoolStripComboBox1.Enabled = this.listView1.Enabled =
                this.searchgroupBox1.Enabled = this.filtergroupBox1.Enabled = changed || (filename != null);
            this.filtergroupBox1.Enabled = this.filtercheckBox.Checked;
        }


        private void saveLogFile()
        {
            StreamWriter writer = null;
            try
            {
                writer = new StreamWriter("file.log");
                writer.WriteLine(filename);

            }
            catch (IOException e)
            {
                MessageBox.Show("Error while saving log file to file:\r\n" + e);

            }
            finally
            {
                if (writer != null)
                    writer.Close();
            }
        }


        private string loadLogFile()
        {
            StreamReader reader = null;
            string line = null;

            try
            {
                reader = new StreamReader("file.log");
                line = reader.ReadLine();
                return line;
            }
            catch (Exception e)
            {
                MessageBox.Show("Error while loading from log file file:\r\n" + e);
                return null;
            }
            finally
            {
                if (reader != null)
                    reader.Close();
            }

        }

        private DataSet1.recordDataDataTable sortBy(DataSet1.recordDataDataTable table, string column_name)
        {
            DataRow[] drarray;
            drarray = table.Select("", column_name);
            DataSet1.recordDataDataTable newTable = new DataSet1.recordDataDataTable();
            for (int i = 0; i < drarray.Length; i++)
            {
                newTable.Rows.Add(drarray[i].ItemArray);
            }
            return newTable;
        }

        private DataSet1.recordDataDataTable sort(DataSet1.recordDataDataTable table)
        {
            DataSet1.recordDataDataTable newTable = new DataSet1.recordDataDataTable();
            switch (this.sortBytoolStripComboBox1.SelectedIndex)
            {
                case 0:
                    {
                        newTable = sortBy(table, "last_name ASC");
                        break;
                    }
                case 1:
                    {
                        newTable = sortBy(table, "last_name DESC");
                        break;
                    }
                case 2:
                    {
                        newTable = sortBy(table, "first_name ASC");
                        break;
                    }
                case 3:
                    {
                        newTable = sortBy(table, "first_name DESC");
                        break;
                    }
            }
            return newTable;
        }

        private DataSet1.recordDataDataTable filterTable(DataSet1.recordDataDataTable table, string expression)
        {
            DataSet1.recordDataDataTable newTable = new DataSet1.recordDataDataTable();
            DataRow[] drarray;
            drarray = table.Select(expression);
            for (int j = 0; j < drarray.Length; j++)
            {
                newTable.Rows.Add(drarray[j].ItemArray);
            }
            return newTable;
        }

        private DataSet1.recordDataDataTable filter()
        {
            DataSet1.recordDataDataTable newTable = new DataSet1.recordDataDataTable();
            string expression = null;
            switch (this.relationtoolStripComboBox1.SelectedIndex)
            {
                case 0:
                    expression = "relation <> ''";
                    break;
                case 1:
                    expression = "relation = 'friends'";
                    break;
                case 2:
                    expression = "relation = 'family'";
                    break;
                case 3:
                    expression = "relation = 'work'";
                    break;
                case 4:
                    expression = "relation = 'other'";
                    break;
            }
            if (filtercheckBox.Checked)
            {
                if (photocheckBox.Checked) { expression += " AND photo <> 'no_photo.jpg'"; }
                if (birthcheckBox.Checked) { expression += " AND birthdate <> ''"; }
                if (emailcheckBox.Checked) { expression += " AND email <> ''"; }
                if (webcheckBox.Checked) { expression += " AND web <> ''"; }
                if (nickcheckBox.Checked) { expression += " AND nickname <> ''"; }
                if (addresscheckBox.Checked) { expression += " AND paddress <> ''"; }
                if (citycheckBox.Checked) { expression += " AND pcity <> ''"; }
                if (companycheckBox.Checked) { expression += " AND nickname <> ''"; }

            }
            newTable = filterTable(this.dataSet1.recordData, expression);
            return newTable;
        }

        private bool createEmptyAddressbook()
        {
            try
            {
                int i = 0;
                while (File.Exists(string.Format("newAddressBook{0}.xml", i))) { i++; }
                NewAdressBook(string.Format("newAddressBook{0}.xml", i));
                counter = 0;
                return true;
            }
            catch (IOException e)
            {
                MessageBox.Show("Error while creating new address book file:\r\n" + e);
                return false;
            }
        }

        private void info()
        {
            MessageBox.Show("PB069\n\rFaculty of Informatics\n\rMasaryk University Brno\n\r\n\rDominik Mačejovský - 359514", "Address Book");
        }

        private void filtercheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (filtercheckBox.Checked)
            {
                filtergroupBox1.Enabled = this.relationtoolStripComboBox1.Enabled =
                    this.sortBytoolStripComboBox1.Enabled = true;
                this.cancelSeacrhbutton.Enabled = false;
                updateListView(sort(filter()));
            }
            else
            {
                filtergroupBox1.Enabled = false;
                updateListView(sort(filter()));
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            this.relationtoolStripComboBox1.SelectedIndex = 0;
            this.sortBytoolStripComboBox1.SelectedIndex = 0;
            this.filtercheckBox.Checked = this.cancelSeacrhbutton.Enabled = false;
            this.sortBytoolStripComboBox1.Enabled = this.relationtoolStripComboBox1.Enabled = true;
            updateListView(sort(filter()));
        }

        private void toolbarToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.toolStrip1.Visible = this.toolbarToolStripMenuItem.Checked;
        }

        private void statusBarToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.statusStrip1.Visible = this.statusBarToolStripMenuItem.Checked;
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            NewAdressBookDialog();
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewAdressBookDialog();
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            SaveFileAs();
        }


        private void searchButton_Click(object sender, EventArgs e)
        {
            this.sortBytoolStripComboBox1.Enabled = this.relationtoolStripComboBox1.Enabled =
                this.filtercheckBox.Checked=this.filtergroupBox1.Enabled=false;
            searchContacts(this.searchTextBox.Text);
            this.cancelSeacrhbutton.Enabled = true;
        }


        private void newContacttoolStripButton1_Click(object sender, EventArgs e)
        {
            newRecord();
        }

        private void newToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            newRecord();
        }

        private void editContactToolStripMenuItem_Click(object sender, EventArgs e)
        {
            editContact();
        }

        private void editContacttoolStripButton1_Click(object sender, EventArgs e)
        {
            editContact();
        }

        private void removeContacttoolStripButton1_Click(object sender, EventArgs e)
        {
            deleteContact();
        }

        private void removeContactToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deleteContact();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openAddressBookViaDialog();
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            openAddressBookViaDialog();
        }

        private void saveasToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileAs();
        }

        private void relationtoolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void sortBytoolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));

        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            contactIndexChanged();
        }

        private void photocheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void birthcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void emailcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void webcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void nickcheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void addresscheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void citycheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void companycheckBox_CheckedChanged(object sender, EventArgs e)
        {
            updateListView(sort(filter()));
        }

        private void cancelSeacrhbutton_Click(object sender, EventArgs e)
        {
            this.sortBytoolStripComboBox1.Enabled = this.relationtoolStripComboBox1.Enabled = true;
            this.cancelSeacrhbutton.Enabled = false;
            updateListView(sort(filter()));
        }

        private void searchTextBox_TextChanged(object sender, EventArgs e)
        {
            this.sortBytoolStripComboBox1.Enabled = this.relationtoolStripComboBox1.Enabled =
                this.filtercheckBox.Checked=this.filtergroupBox1.Enabled=false;
            searchContacts(this.searchTextBox.Text);
            this.cancelSeacrhbutton.Enabled = true; ;
        }

        private void helpToolStripButton_Click(object sender, EventArgs e)
        {
            info();
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            info();
        }

        private void searchTextBox_Enter(object sender, EventArgs e)
        {
            this.sortBytoolStripComboBox1.Enabled = this.relationtoolStripComboBox1.Enabled =
                this.filtercheckBox.Checked = this.filtergroupBox1.Enabled = false;
            searchContacts(this.searchTextBox.Text);
            this.cancelSeacrhbutton.Enabled = true; ;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void MainMenu_FormClosing(object sender, FormClosingEventArgs e)
        {
            DialogResult dlg = MessageBox.Show("Do you really want to quit?", "Warning", MessageBoxButtons.YesNo);
            if (dlg == DialogResult.No)
                e.Cancel = true;
            else if (dlg == DialogResult.No)
                e.Cancel = false;
        }


    }
}
