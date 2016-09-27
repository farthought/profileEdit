#include "widget.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{

	filename.clear();
	isSaved = 0;

    this->resize(600,400);


	label_fineName = new QLabel("NO File!");
	tableView = new QTableView();
	button_Read = new QPushButton("Load");
	button_Save = new QPushButton("Save"),
	button_Exit = new QPushButton("Exit");
	button_Addcol = new QPushButton("Col");
	button_Addrow = new QPushButton("Row");

	//add 2015年7月6日,适应Section变化的需求
	combox = new QComboBox(this);

	QVBoxLayout *V1 = new QVBoxLayout();
	QHBoxLayout *H1 = new QHBoxLayout();
	QHBoxLayout *H2 = new QHBoxLayout();
	QHBoxLayout *H3 = new QHBoxLayout();
	QHBoxLayout *H4 = new QHBoxLayout();
	QHBoxLayout *H5 = new QHBoxLayout();

	button_Read->setMinimumSize(10,5);
	button_Save->setMinimumSize(10,5);
	button_Exit->setMinimumSize(10,5);

	button_Addcol->setMinimumSize(10,5);
	button_Addrow->setMinimumSize(10,5);


	H1->addWidget(button_Read);
	H1->addWidget(button_Save);
	H2->addWidget(button_Addrow);
	H2->addWidget(button_Addcol);
	H3->addWidget(button_Exit);

	H4->addLayout(H1);
	H4->addLayout(H2);
	H4->addLayout(H3);

	combox->resize(20,10);

    label_fineName->setMinimumSize(100,10);
	H5->addWidget(combox);
	H5->addWidget(label_fineName);

	V1->addLayout(H5);
	V1->addWidget(tableView);
	V1->addLayout(H4);



	connect (button_Read, SIGNAL(clicked()), this, SLOT(on_button_Read_clicked()));
	connect (button_Save, SIGNAL(clicked()), this, SLOT(on_button_Save_clicked()));
	connect (button_Exit, SIGNAL(clicked()), this, SLOT(on_button_Exit_clicked()));
	connect (button_Addcol,SIGNAL(clicked()), this, SLOT(on_button_Addcol_clicked()));
	connect (button_Addrow, SIGNAL(clicked()), this, SLOT(on_button_Addrow_clicked()));
    connect (combox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_combox_currentTextchange(QString)));

	this->setLayout(V1);
	this->show();
}

Widget::~Widget()
{

}


void Widget::tableview_setmodel(QStandardItemModel *model)
{
	tableView->setModel(model);

	//表头信息显示居中
	tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

	//设置列宽
	tableView->setColumnWidth(0,200);

	tableView->setColumnWidth(1,100);
}


void Widget::on_button_Read_clicked()
{
	//QString section("FPGA3");
    filename = QFileDialog::getOpenFileName(this,tr("Open profile"), "/home/sg-os/", tr("profile(*.ini)"));
	if (0 == filename.length()) {
		QMessageBox::information(NULL, tr("Profile"), tr("You didn't select any files."));
		label_fineName->setText("NO file!");
		return ;
	} else {
		//QMessageBox::information(NULL, tr("Profile"), tr("You selected ") + filename);
		label_fineName->setText(filename);
	}
	combox->clear();

	isSaved = 0;
//	int x = 0;
//	int y = 0;

	parse_ini(filename);
	store_to_model();
	foreach(QString tmp, sectionList){
		combox->addItem(tmp);
	}

//	MODELLIST modelList;

//	//added by jiawentao ,new style to save ini data special section
//	//calculate section
//	QStringList sectionList;
//	modelList.clear();
//	for (x = 0; x != keys.size(); x++) {
//		if(!sectionList.contains(keys[x].section)) {
//			sectionList.append(keys[x].section);
//			struct __model *tmp;
//			tmp.section = keys[x].section;
//			tmp.model.clear();
//			modelList.append(tmp);
//		}
//	}


//	QSettings *configIniRead = new QSettings(filename, QSettings::IniFormat);


//	QStringList Keylist = configIniRead->allKeys();

//	QStringList valueList;
//	QStringList::Iterator it1,it2;
//	QString str1, str2;
//	int x = 0;
//	int y = 0;

//	//foreach (QString str, Keylist) {
//	//for(it1 = Keylist.begin(); it1 != Keylist.end(); it1++) {
//	for (int i = 0; i < Keylist.size(); i++) {

//		y = 0;
//		//str1 = *it1;
//		str1 = Keylist[i];
//		model->setItem(x, y, new QStandardItem(QString(str1).right(str1.length() - str1.indexOf('/') -1 )));

//		valueList = configIniRead->value(str1).toStringList();
//		//foreach (QString str2, valueList) {
//		for (it2 = valueList.begin(); it2 != valueList.end(); it2++) {
//			y++;
//			str2 = *it2;
//			model->setItem(x, y, new QStandardItem(QString(str2)));
//		}
//		x++;
//		qDebug()<<str1<<valueList;
//	}

//	//读入入完成后删除指针
//	delete configIniRead;
}

void Widget::on_button_Save_clicked()
{

	QString Key;
	QStringList ValueList;
	int column = 1;
	QStandardItemModel *model = NULL;
	filename = QFileDialog::getSaveFileName(this,tr("Save profile"), filename, tr("profile(*.ini)"));
	if (filename.isNull()) {
		return ;
	}

	QSettings *configIniSave = new QSettings(filename, QSettings::IniFormat);
	configIniSave->clear();
	//configIniSave->beginGroup(section);
	for (int index = 0; index < modelList.size(); index++) {
		model = &modelList[index]->model;
		for (int row =0 ;row < model->rowCount(); row++) {
			//qDebug()<<"-----"<<model->item(row, 0)->text();
			int sum = 0;
			Key.clear();
			ValueList.clear();
			if (model->item(row, 0) == NULL)
				continue;
			Key = model->item(row,0)->text();
			if (Key.isNull() || Key.isEmpty() || Key == "\r\n" || Key == "\n")
				continue;
			Key = modelList[index]->section + "/" + Key;
			for (column = 1; column < model->columnCount(); column++){
				if (model->item(row, column) == NULL || model->item(row, column)->text().isEmpty() || model->item(row, column)->text() == "\r\n")
					continue;
				else
					sum++, ValueList.append(model->item(row, column)->text());
			}
			if (sum == 0)
				continue;

			qDebug()<<"save"<<Key<<ValueList;
			configIniSave->setValue(Key, ValueList);
		}
	}
	//configIniSave->endGroup();

	delete configIniSave;
	configIniSave = NULL;

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
	QStandardItemModel *model = NULL;
	model = get_model(combox->currentText());
	if (model->columnCount()==0)
		tableview_setmodel(model);
	model->insertColumn(model->columnCount());
	//qDebug()<<model->columnCount();
}

void Widget::on_button_Addrow_clicked()
{
	QStandardItemModel *model = NULL;
	model = get_model(combox->currentText());
	if (model->columnCount()==0)
		tableview_setmodel(model);
	model->insertRow(model->rowCount());
	//qDebug()<<model->rowCount();
}
QStandardItemModel* Widget::get_model(QString section)
{
	if (section.isNull() || section.isEmpty())
		return NULL;
	foreach (struct __model * tmp, modelList) {
		if (tmp->section == section) {
			return &tmp->model;
		}
	}
	return NULL;
}

void Widget::store_to_model()
{
	//free memory
	qDeleteAll(modelList);
	modelList.clear();
	sectionList.clear();

	//calculate section, create model
	for (int index = 0; index != keyList.size(); index++) {
		if(!sectionList.contains(keyList.at(index).section)) {
			sectionList.append(keyList.at(index).section);
			struct __model *tmp = new struct __model;
			tmp->section = keyList.at(index).section;
			tmp->model.clear();
			//设置表头
			tmp->model.setColumnCount(2);
			tmp->model.setHeaderData(0,Qt::Horizontal,QString("Params "));
			tmp->model.setHeaderData(1,Qt::Horizontal,QString("Value"));

			modelList.append(tmp);
		}
	}
	//	//从keylist中拿数据
	//	for (int x = 0, cow = 0; x != keyList.size(); x++) {
	//		if (keyList[x].section == str) {
	//			int y = 0;
	//			model->setItem(cow, y, new QStandardItem(keyList[x].key));

	//			foreach (int value, keyList[x].valuelist) {
	//				y++;
	//				model->setItem(cow, y, new QStandardItem(QString::number(value)));

	//			}
	//			cow++;
	//		}
	//	}

	//save data to model
	struct __model *tmp = NULL;
	foreach(tmp, modelList) {
		for (int x = 0, cow = 0; x != keyList.size(); x++) {
			if (keyList[x].section == tmp->section){
				int y = 0;
				tmp->model.setItem(cow, y, new QStandardItem(keyList[x].key));
				foreach(int value, keyList[x].valuelist) {
					y++;
					tmp->model.setItem(cow, y, new QStandardItem(QString::number(value)));
				}
				cow++;
			}
		}
	}


}

void Widget::parse_ini(QString filename)
{
	QString curLine;
	QString section;

	struct _key tmpKey;

	int sum = 0;
	QFile *ini = new QFile(filename);
	if (!ini->open(QFile::ReadOnly)){
		qDebug()<<"Open file error!";
		return ;
	}
	keyList.clear();

	while(!ini->atEnd()) {

		curLine = QString (ini->readLine());
		qDebug()<<"Current Line :"<<curLine;

		//section,such as "[FPGA3]"
		if (curLine.contains('[') && curLine.contains(']')) {
			section = curLine.right(curLine.size() - curLine.indexOf('[') - 1) ;
			section = section.left(section.lastIndexOf(']'));
			qDebug()<<"get section"<<section;
			continue;
		}

		//Key,values
		if (curLine.contains('=')) {

			tmpKey.section = section;
			tmpKey.valuelist.clear();

			tmpKey.key = curLine.split('=').first();
			if (tmpKey.key.isEmpty())
				continue;

			//',' 逗号作为分隔符
			if (curLine.size() - tmpKey.key.size() - 1 <=0)
				continue;

			QStringList tmp = curLine.right(curLine.size() - tmpKey.key.size() - 1).split(',');
			if (tmp.isEmpty() || tmp.first() == "\r\n" ||tmp.first() == "\n") {
				continue;
			}
			//只保留数字，过滤其它字符
			foreach (QString s, tmp) {
				QString num;
				for (int index = 0; index !=s.size(); index++) {
					if (s[index].isDigit())
						num.append(s[index]);
				}
				tmpKey.valuelist.append(num.toLong());
			}

			keyList.append(tmpKey);
			//()<<keys->value(sum).section<<keys->value(sum).key<<keys->value(sum).valuelist;
			sum++;
		}
	}
	ini->close();

	return ;
}
void Widget::on_combox_currentTextchange(QString str)
{
	//show
	int index = 0;
	//找到用来填数据的目标model
	QStandardItemModel *model = NULL;
	if (str.isNull() || str.isEmpty())
		return ;
	while(index < modelList.size() && str!= modelList[index]->section){
		index++;
	}
	if (index < modelList.size() && str == modelList[index]->section) {
		model = &modelList[index]->model;
	}
	//替换UI中的model
	tableview_setmodel(model);

}
