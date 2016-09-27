#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QDebug>
#include <QTableView>
#include <QPushButton>
#include <QList>
#include <QChar>
#include <QString>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QComboBox>

struct _key {
	QString section;
	QString key;
	QList<int> valuelist;
};
typedef QList<struct _key> KEYLIST;

struct __model {
	QString section;
	QStandardItemModel model;
};
typedef QList<struct __model *> MODELLIST;

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = 0);
	~Widget();

	//widget
	QLabel *label_fineName;
	QTableView *tableView;
	QPushButton *button_Read, *button_Save, *button_Exit;

	QPushButton *button_Addrow, *button_Addcol;

	//add 2015年7月6日
	QComboBox *combox;

	//file
	QString filename;

	//Data
	KEYLIST keyList;
	MODELLIST modelList;
	QStringList sectionList;

	//model
	void tableview_setmodel(QStandardItemModel *model);

	//parse, the QSettings API 'allKeys' get a QStringList was been sort,
	// i can't avoid the sort,so i rewrite a parse function
	void parse_ini(QString filename);

	void store_to_model();

	QStandardItemModel* get_model(QString section);

	//flag
	int isSaved;
private:

private slots:
	void on_button_Read_clicked();
	void on_button_Save_clicked();
	void on_button_Exit_clicked();
	void on_button_Addrow_clicked();
	void on_button_Addcol_clicked();
	void on_combox_currentTextchange(QString );
};

#endif // WIDGET_H
