#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSharedPointer>
#include <QTableWidgetItem>
#include "pb/app_window.pb.h"
#include "network/tcp_server_impl.h"
#include "float_cover_mask.h"
#include "object_node_model.h"
#include "client_selector.h"
#include "show_uricontent_dlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE
struct DialogInfo {
  int screen_width;
  int screen_height;
  QPixmap pixmap;
};

class Dialog : public QDialog, public TcpServerImplDelegate, public TcpClientDelegate {
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

Q_SIGNALS:
    void receivedPacket(TcpClientImpl* client,
                        int16_t packet_type,
                        int packet_serial,
                        const std::string& body);

    void removeClient(TcpClientImpl* client);

private:
  void OnNewConnection(TcpClientImpl* client) override;
  void OnShutdown(QAbstractSocket::SocketError error, const QString& message) override;

private:
  void OnConnected(TcpClientImpl* client) override;
  void OnDisConnected(TcpClientImpl* client,
                                QAbstractSocket::SocketError error,
                        const QString& message) override;
  void OnPacketReceived(TcpClientImpl* client,
                      int16_t packet_type,
                        int packet_serial,
                      std::string&& body) override;
  void closeEvent(QCloseEvent* e) override;

  void resizeEvent(QResizeEvent*) override;

private:
  void HandlePacket(int16_t packet_type, int packet_serial, std::string&& body);
 void HandleWindowInfo(int packet_serial, std::string&& body);
  void HandleUriContent(int packet_serial, std::string&& body);
  
  void ShowMask(const MetaObjectDetail& detail);
  void SendPacket(pb::PacketType type,
                  std::string&& body,
                  TcpClientImpl::SendPacketDelegate delegate = nullptr);
  void EnableUI();
  DialogInfo ConvertWindowInfo(const ::pb::WindowInfo& window_info);
  void ShowDialogImageIfNeed();

 private slots:
  void OnClickNode(const QModelIndex& proxy_index);
    void on_refresh_clicked();

    void OnClientSelected(TcpClientImpl* client);

    void on_show_clients_clicked();
    void OnClickChangeColor();
    void OnClickChangeFont();
    void OnClickBrowser();
    void OnProperyValueStateChange(int state);
    void OnPropertyValueEdit(int row, int column);

    void on_resource_manager_clicked();

    void on_search_text_returnPressed();

    void on_expand_clicked();

    void on_collapse_clicked();

private:
    void InnerSetProperty(const QString& property_name,
                          const QVariant& value,
                          TcpClientImpl::SendPacketDelegate delegate = nullptr);
    void InitPropertyTable(const MetaObjectDetail& detail);
    void UpdateClientSelector(TcpClientImpl* client, const std::string& body);
    void ShowPropertyValue(const QString& property_name, int row, const QVariant& value);


    void HandleBoolProperty(const QString& property_name,
                            int row,
                            const QVariant& value);
    void HandleColorProperty(const QString& property_name,
                            int row,
                            const QVariant& value);

    void HandleFontProperty(const QString& property_name,
                             int row,
                             const QVariant& value);

    
    void HandleUrlProperty(const QString& property_name,
                            int row,
                            const QVariant& value);
    

    void HandleDefaultProperty(const QString& property_name,
                             int row,
                             const QVariant& value);

private:
    Ui::Dialog *ui;
    QSharedPointer<TcpServerImpl> tcp_server_;
    QList<TcpClientImpl*> clients_;
    TcpClientImpl* current_client_ = nullptr;
    std::unique_ptr<FloatCoverMask> cover_mask_;
    float screen_image_scale_ = 1.0f;
    QtObjectNode* current_select_node = nullptr;
    std::unique_ptr<ClientSelector> client_selector_;
    std::unique_ptr<ShowUriContentDlg> show_uri_content;
    QMap<QtObjectNode*, DialogInfo> root_window_info_;
    std::unique_ptr<ObjectSearchProxyModel> object_proxy_model_;
};
#endif // DIALOG_H