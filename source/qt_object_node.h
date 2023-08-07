#pragma once
#include <QObject>
#include <QList>
#include "meta_object_detail.h"
class QtObjectNode {
 public:
  QtObjectNode(QObject* object, QtObjectNode* parent, int window_unique_id);
  QtObjectNode(QtObjectNode* parent) : parent_(parent) {};
  ~QtObjectNode();

public:
  const MetaObjectDetail* object_detail() const { 
    return object_detail_;
  }

  const QList<QtObjectNode*>& children() const { 
    return children_;
  }

  void AddChild(QtObjectNode* child) { 
    children_.push_back(child);
  }

  const QtObjectNode* parent() const {
    return parent_;
  }

  QtObjectNode* FindQObjectByUniqueId(int unique_id);

  int window_unique_id() const { return window_unique_id_; }

  int unique_id() const { return unique_id_; }

  void SetProperty(const QString& prop_name, const QVariant& param);
  void Refresh();

public:
  QJsonObject DumpToJson();
  QString DumpToString();

  friend QDataStream& operator<<(QDataStream& stream, const QtObjectNode& node);
  friend QDataStream& operator>>(QDataStream& stream, QtObjectNode& node);

 private:
  void FindChildrenObject(QList<QtObjectNode*>& node_list, QObject* object);

  static QJsonObject InnerDump(const QtObjectNode& node);

private:
  QObject* object_pointer_ = nullptr;
  MetaObjectDetail* object_detail_ = nullptr;
  QList<QtObjectNode*> children_;
  QtObjectNode* parent_ = nullptr;
  int unique_id_ = 0;
  int window_unique_id_ = 0;
};