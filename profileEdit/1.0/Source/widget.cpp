#include "widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{

	filename.clear();
	isSaved = 0;

	this->resize(1024,768);


	label_fineName = new QLabel("NO File!");
	tableView = new QTableView();
	button_Read = new QPushButton("Load");
	button_Save = new QPushButton("Save"),
	button_Exit = new QPushButton("Exit");
	button_Addcol = new QPushButton("Col");
	button_Addrow = new QPushButton("Row");

	model = new QStandardItemModel(tableView);
	QVBoxLayout *V1 = new QVBoxLayout();
	QHBoxLayout *H1 = new QHBoxLayout();
	QHBoxLayout *H2 = new QHBoxLayout();

	button_Read->setMinimumSize(10,5);
	button_Save->setMinimumSize(10,5);
	button_Exit->setMinimumSize(10,5);

	button_Addcol->setMinimumSize(10,5);
	button_Addrow->setMinimumSize(10,5);


	H1->addWidget(button_Read);
	H1->addWidget(button_Save);
	H1->addWidget(button_Addrow);
	H1->addWidget(button_Addcol);
	H1->addWidget(button_Exit);

	label_fineName->resize(600,20);
	H2->addWidget(label_fineName);
	H2->addLayout(H1);


	V1->addWidget(tableView);
	V1->addLayout(H2);



	connect (button_Read, SIGNAL(clicked()), this, SLOT(on_button_Read_clicked()));
	connect (button_Save, SIGNAL(clicked()), this, SLOT(on_button_Save_clicked()));
	connect (button_Exit, SIGNAL(clicked()), this, SLOT(on_button_Exit_clicked()));
	connect (button_Addcol,SIGNAL(clicked()), this, SLOT(on_button_Addcol_clicked()));
	connect (button_Addrow, SIGNAL(clicked()), this, SLOT(on_button_Addrow_clicked()));


	this->setLayout(V1);
	this->show();
}

Widget::~Widget()
{

}


void Widget::model_init()
{
	//设置表头

	model->setColumnCount(2);

	model->setHeaderData(0,Qt::Horizontal,QString("Params "));

	model->setHeaderData(1,Qt::Horizontal,QString("Value"));

	tableView->setModel(model);

	//表头信息显示居左
	tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

	//设置列宽
	tableView->setColumnWidth(0,200);

	tableView->setColumnWidth(1,100);
}

void Widget::on_button_Read_clicked()
{
	filename = QFileDialog::getOpenFileName(this,tr("Open profile"), ".", tr("profile(*.ini)"));
	if (0 == filename.length()) {
		QMessageBox::information(NULL, tr("Profile"), tr("You didn't select any files."));
		label_fineName->setText("NO file!");
		return ;
	} else {
		//QMessageBox::information(NULL, tr("Profile"), tr("You selected ") + filename);
		label_fineName->setText(filename);
	}
	model->clear();
	model_init();
	isSaved = 0;

	QSettings *configIniRead = new QSettings(filename, QSettings::IniFormat);

	QStringList Keylist = configIniRead->allKeys();

	QStringList valueList;
	int x = 0;
	int y = 0;
	foreach (QString str, Keylist) {
		y = 0;
		model->setItem(x, y, new QStandardItem(QString(str).right(str.length() - str.indexOf('/') -1 )));

		valueList = configIniRead->value(str).toStringList();
		foreach (QString str2, valueList) {
			y++;
			model->setItem(x, y, new QStandardItem(QString(str2)));
		}
		x++;
		//qDebug()<<str<<valueList;
	}

	//读入入完成后删除指针
	delete configIniRead;
}

void Widget::on_button_Save_clicked()
{

	QString section("FPGA3");
	QString Key;
	QStringList ValueList;
	int column = 1;

	filename = QFileDialog::getSaveFileName(this,tr("Save profile"), filename, tr("profile(*.ini)"));
	if (filename.isNull()) {
		return ;
	}

	QSettings *configIniSave = new QSettings(filename, QSettings::IniFormat);
	configIniSave->clear();
	configIniSave->beginGroup(section);
	//qDebug()<<model->rowCount()<<model->columnCount();


	for (int row =0 ;row < model->rowCount(); row++){
		//qDebug()<<"-----"<<model->item(row, 0)->text();
		Key.clear();
		ValueList.clear();
		Key = model->item(row,0)->text();
		if (Key.isNull())
			continue;
		column = 1;
		while ( column < model->columnCount()){
			if (model->item(row, column) == NULL)
				break;
			else
				ValueList.append(model->item(row, column)->text());
			column++;
		}

		configIniSave->setValue(Key, ValueList);
	}
	configIniSave->endGroup();
	delete configIniSave;
	isSaved = 1;
	label_fineName->setText(filename);
}

void Widget::on_button_Exit_clicked()
{
	if (isSaved == 0) {
		int ret = QMessageBox::information(this,"Not Save", "Don't save and Exit?",QMessageBox::Save, QMessageBox::Yes,QMessageBox::No );
		switch (ret) {
			case QMessageBox::Save:
				on_button_Save_clicked();
				break;
			case QMessageBox::Yes:
				this->close();
				break;
			case QMessageBox::No:
				break;
			default:
				break;
		}
	} else {
		this->close();
	}
}

void Widget::on_button_Addcol_clicked()
{
	if (model->columnCount()==0)
		model_init();
	model->insertColumn(model->columnCount());
	qDebug()<<model->columnCount();
}

void Widget::on_button_Addrow_clicked()
{
	if (model->columnCount()==0)
		model_init();
	model->insertRow(model->rowCount());
	qDebug()<<model->rowCount();
}
