#include "ChangePassword.h"

#include <QMessageBox>
#include "BusyIndicator.h"

JChat::ChangePassword::ChangePassword(ClientObjectPtr const& co,  QWidget *parent)
	: QWidget(parent,Qt::Window)
	, _co(co)
{
	ui.setupUi(this);

	this->setWindowModality(Qt::WindowModal);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setFixedSize(size());
}

JChat::ChangePassword::~ChangePassword()
{
}

Q_SLOT void
JChat::ChangePassword::on_btnOK_clicked()
{

	auto newPwd1 = ui.lineEditNewPwd1->text();
	auto newPwd2 = ui.lineEditNewPwd2->text();

	if(newPwd1 != newPwd2)
	{
		QMessageBox::warning(this, "", u8"���������벻һ��!", QMessageBox::Ok);
		return;
	}
	auto oldPwd = ui.lineEditOldPwd->text();

	if(oldPwd.isEmpty() || newPwd1.isEmpty() || newPwd2.isEmpty())
	{
		QMessageBox::warning(this, "", u8"���벻��Ϊ��!", QMessageBox::Ok);
		return;
	}

	try
	{
		BusyIndicator busy(this);
		qAwait(_co->updateSelfPassword(oldPwd.toStdString(), newPwd1.toStdString()));
		QMessageBox::warning(this, "", u8"�޸ĳɹ�!", QMessageBox::Ok);
		close();
	}
	catch(Jmcpp::ServerException& e)
	{
		if (e.code()== 882002)
		{
			QMessageBox::warning(this, "", u8"ԭ���벻��ȷ!", QMessageBox::Ok);
		}
		else
		{
			QMessageBox::warning(this, "", e.what(), QMessageBox::Ok);
		}
	}
	catch(std::runtime_error& e)
	{
		QMessageBox::warning(this, "", e.what(), QMessageBox::Ok);
	}
}

Q_SLOT void
JChat::ChangePassword::on_btnCancel_clicked()
{
	close();
}


