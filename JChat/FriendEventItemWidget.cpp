#include "FriendEventItemWidget.h"
#include <QMouseEvent>

#include "Util.h"
namespace JChat {

	FriendEventItemWidget::FriendEventItemWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);

		ui.label->installEventFilter(this);


		connect(ui.toolButtonPass, &QToolButton::clicked, this, [=]
		{
			Q_EMIT passClicked(_userId, _id);

		});
		connect(ui.toolButtonReject, &QToolButton::clicked, this, [=]
		{
			Q_EMIT rejectClicked(_userId, _id);
		});

	}

	FriendEventItemWidget::~FriendEventItemWidget()
	{

	}

	None FriendEventItemWidget::setEvent(ClientObjectPtr co, FriendEventT et)
	{
		auto userId = Jmcpp::UserId{ et.username ,et.appkey };
		_userId = userId;
		_id = et.id;

		auto self = this | qTrack;

		Jmcpp::UserInfo info;
		info.userId = userId;
		try
		{
			info = co_await co->getCacheUserInfo(userId);
		}
		catch(...)
		{
		}

		auto name = getUserDisplayName(info);

		co_await self;
		setStatus(et.isOutgoing, et.status);

		if(et.isOutgoing)
		{
			setTitleLabel(name);

			auto text = et.status == et.undone ? u8"�ȴ��Է���֤"
				: et.status == et.passed ? u8"ͬ�������ĺ�������"
				: u8"�ܾ������ĺ�������";

			setStatusLabel(text);

		}
		else
		{
			auto text = QString(u8"%1 ���������Ϊ����").arg(name);
			setTitleLabel(text);

			text = et.status == et.undone ? u8""
				: et.status == et.passed ? u8"��ͬ��"
				: u8"�Ѿܾ�";

			setStatusLabel(text);
		}

		setMessageLabel(et.message);

		auto pixmap = co_await co->getCacheUserAvatar(userId);
		co_await self;
		setPixmap(pixmap);
	}

	bool FriendEventItemWidget::eventFilter(QObject *watched, QEvent *event)
	{
		if(event->type() == QEvent::MouseButtonRelease)
		{
			auto ev = static_cast<QMouseEvent*>(event);
			if(ev->button() == Qt::LeftButton)
			{
				Q_EMIT infoClicked(_userId, _id);
			}
		}
		return false;
	}

} // namespace JChat
