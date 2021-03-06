﻿#pragma once



#include <QPainter>
#include <QTextItem>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

#include <Jmcpp/Client.h>

#include "MetaTypes.h"
#include "await.h"

#include <QxOrm.h>

namespace JChat{

	class ClientObject;

	class ConversationModel :public QStandardItemModel
	{
		Q_OBJECT
	public:
		enum Role
		{
			ConIdRole = Qt::UserRole + 222,
			ImageRole,
			TitleRole,
			TimeRole,
			MessageRole,

			NotDisturb,
			ShieldRole,
			UnreadRole,

			SticktopTimeRole
		};


		ConversationModel(ClientObject* co, QObject* parent = nullptr);

		QStandardItem*  addConversationItem(Jmcpp::ConversationId const& conId);

		void			addConversationItem(Jmcpp::MessagePtr const& msg, bool successed = true);

		void			setSticktop(Jmcpp::ConversationId const& conId, bool enabled);

		void			setMessageRole(Jmcpp::ConversationId const& conId, QString const&);

		void			updateMessageRole(Jmcpp::ConversationId const& conId);

		void			setUnreadMessageCountRole(Jmcpp::ConversationId const& conId, int unreadMsgCount);

		void			removeRow(int row);

		void			remove(Jmcpp::ConversationId const& conId);

		int				getTotalUnreadMessageCount() const;

		Q_SIGNAL void	unreadMessageCountChanged(Jmcpp::ConversationId const& conId, int unreadMsgCount);

		static void		sortFn(const QModelIndex &source_left, const QModelIndex &source_right, bool& ret);

	protected:
		void			_addConversationToDB(Jmcpp::MessagePtr const& msg);


		None	_updateItem(QStandardItem* item);

		None	_updateMessagRole(QStandardItem* item);

		None	_updateMessagRole(QStandardItem* item, Jmcpp::MessagePtr msg, bool successed = true);



		void			onUserInfoUpdated(Jmcpp::UserId const& userId);



		None			_updateItemUserInfo(QStandardItem* item, Jmcpp::UserId userId);

		None			_updateItemGroupInfo(QStandardItem* item, Jmcpp::GroupId groupId);

	protected:
		Q_SLOT None onUserLogined();

	private:
		ClientObject* _co;

		friend class ConversationDelegate;
	};




}

