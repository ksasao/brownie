namespace BrownieMonitor
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージド リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panel1 = new System.Windows.Forms.Panel();
            this.labelInfo = new System.Windows.Forms.Label();
            this.textBoxCOMPort = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.buttonSave = new System.Windows.Forms.Button();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.dataGridViewCommand = new System.Windows.Forms.DataGridView();
            this.ifDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.thenDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.commandItemBindingSource2 = new System.Windows.Forms.BindingSource(this.components);
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.executeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCommand)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.commandItemBindingSource2)).BeginInit();
            this.contextMenuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.labelInfo);
            this.panel1.Controls.Add(this.textBoxCOMPort);
            this.panel1.Controls.Add(this.buttonConnect);
            this.panel1.Controls.Add(this.buttonSave);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1676, 155);
            this.panel1.TabIndex = 0;
            // 
            // labelInfo
            // 
            this.labelInfo.AutoSize = true;
            this.labelInfo.Font = new System.Drawing.Font("源ノ角ゴシック JP Bold", 24F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.labelInfo.Location = new System.Drawing.Point(333, 29);
            this.labelInfo.Name = "labelInfo";
            this.labelInfo.Size = new System.Drawing.Size(112, 95);
            this.labelInfo.TabIndex = 3;
            this.labelInfo.Text = "---";
            // 
            // textBoxCOMPort
            // 
            this.textBoxCOMPort.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxCOMPort.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.textBoxCOMPort.Location = new System.Drawing.Point(1249, 51);
            this.textBoxCOMPort.Multiline = true;
            this.textBoxCOMPort.Name = "textBoxCOMPort";
            this.textBoxCOMPort.Size = new System.Drawing.Size(145, 46);
            this.textBoxCOMPort.TabIndex = 2;
            this.textBoxCOMPort.Text = "COM1";
            this.textBoxCOMPort.TextChanged += new System.EventHandler(this.TextBoxCOMPort_TextChanged);
            // 
            // buttonConnect
            // 
            this.buttonConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonConnect.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.buttonConnect.Location = new System.Drawing.Point(1432, 39);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(202, 62);
            this.buttonConnect.TabIndex = 1;
            this.buttonConnect.Text = "&Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.ButtonConnect_Click);
            // 
            // buttonSave
            // 
            this.buttonSave.Location = new System.Drawing.Point(31, 45);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(167, 56);
            this.buttonSave.TabIndex = 0;
            this.buttonSave.Text = "&Save";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.ButtonSave_Click);
            // 
            // textBoxLog
            // 
            this.textBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxLog.Font = new System.Drawing.Font("源ノ角ゴシック JP Regular", 13.875F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.textBoxLog.Location = new System.Drawing.Point(0, 0);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLog.Size = new System.Drawing.Size(1676, 227);
            this.textBoxLog.TabIndex = 0;
            // 
            // dataGridViewCommand
            // 
            this.dataGridViewCommand.AutoGenerateColumns = false;
            this.dataGridViewCommand.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.dataGridViewCommand.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("源ノ角ゴシック JP Heavy", 19.875F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewCommand.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridViewCommand.ColumnHeadersHeight = 90;
            this.dataGridViewCommand.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ifDataGridViewTextBoxColumn,
            this.thenDataGridViewTextBoxColumn});
            this.dataGridViewCommand.DataSource = this.commandItemBindingSource2;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("源ノ角ゴシック JP Bold", 19.875F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dataGridViewCommand.DefaultCellStyle = dataGridViewCellStyle2;
            this.dataGridViewCommand.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewCommand.EnableHeadersVisualStyles = false;
            this.dataGridViewCommand.Location = new System.Drawing.Point(0, 0);
            this.dataGridViewCommand.Name = "dataGridViewCommand";
            dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle3.BackColor = System.Drawing.SystemColors.Control;
            dataGridViewCellStyle3.Font = new System.Drawing.Font("源ノ角ゴシック JP Bold", 19.875F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            dataGridViewCellStyle3.ForeColor = System.Drawing.SystemColors.WindowText;
            dataGridViewCellStyle3.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle3.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle3.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.dataGridViewCommand.RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
            this.dataGridViewCommand.RowHeadersWidth = 40;
            this.dataGridViewCommand.RowTemplate.Height = 33;
            this.dataGridViewCommand.Size = new System.Drawing.Size(1676, 500);
            this.dataGridViewCommand.TabIndex = 2;
            // 
            // ifDataGridViewTextBoxColumn
            // 
            this.ifDataGridViewTextBoxColumn.DataPropertyName = "If";
            this.ifDataGridViewTextBoxColumn.HeaderText = "If";
            this.ifDataGridViewTextBoxColumn.MinimumWidth = 10;
            this.ifDataGridViewTextBoxColumn.Name = "ifDataGridViewTextBoxColumn";
            this.ifDataGridViewTextBoxColumn.Width = 118;
            // 
            // thenDataGridViewTextBoxColumn
            // 
            this.thenDataGridViewTextBoxColumn.DataPropertyName = "Then";
            this.thenDataGridViewTextBoxColumn.HeaderText = "Then";
            this.thenDataGridViewTextBoxColumn.MinimumWidth = 10;
            this.thenDataGridViewTextBoxColumn.Name = "thenDataGridViewTextBoxColumn";
            this.thenDataGridViewTextBoxColumn.Width = 214;
            // 
            // commandItemBindingSource2
            // 
            this.commandItemBindingSource2.DataSource = typeof(BrownieMonitor.CommandItem);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.executeToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(173, 42);
            // 
            // executeToolStripMenuItem
            // 
            this.executeToolStripMenuItem.Name = "executeToolStripMenuItem";
            this.executeToolStripMenuItem.Size = new System.Drawing.Size(172, 38);
            this.executeToolStripMenuItem.Text = "&Execute";
            this.executeToolStripMenuItem.Click += new System.EventHandler(this.ExecuteToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 155);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.dataGridViewCommand);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.textBoxLog);
            this.splitContainer1.Size = new System.Drawing.Size(1676, 731);
            this.splitContainer1.SplitterDistance = 500;
            this.splitContainer1.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 24F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1676, 886);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.Text = "Brownie Monitor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewCommand)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.commandItemBindingSource2)).EndInit();
            this.contextMenuStrip1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.DataGridView dataGridViewCommand;
        private System.Windows.Forms.DataGridViewTextBoxColumn ifDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn thenDataGridViewTextBoxColumn;
        private System.Windows.Forms.BindingSource commandItemBindingSource2;
        private System.Windows.Forms.Button buttonSave;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.TextBox textBoxCOMPort;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem executeToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Label labelInfo;
    }
}

