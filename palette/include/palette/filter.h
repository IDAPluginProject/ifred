#ifndef PALETTE_FILTER_H
#define PALETTE_FILTER_H

#include <QtGui>
#include <QtWidgets>

#include <palette/action.h>

class SearchService;

class PaletteFilter : public QAbstractItemModel
{
    Q_OBJECT;

    QVector<Action> shown_items_;
    QString keyword_;
    SearchService* search_service_;

public:
    PaletteFilter(QWidget* parent, const QString& palette_name, SearchService* search_service);

    // Public interface
    void setFilter(const QString& keyword);
    const QString& filter() { return keyword_; }

    SearchService* search_service()
    {
        return search_service_;
    }

    void set_search_service(SearchService* new_service)
    {
        search_service_ = new_service;
    }

    // Implementations
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QModelIndex parent(const QModelIndex& index) const override { return {}; }

    int columnCount(const QModelIndex& parent) const override { return 1; }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

public slots:
    void onDoneSearching(QString keyword, QVector<Action> items, int recent_count);

signals:
    void filteringDone(int recent_count);
};

class SearchService : public QObject
{
    Q_OBJECT;

public:
    using QObject::moveToThread;
    SearchService(QObject* parent) : QObject(parent) {};

    void search(QString keyword) {
        cancel();
        emit startSearching(keyword);
    }

    virtual void cancel() = 0;

signals:
    // Request
    void startSearching(const QString& keyword);
    void itemClicked(const QString& action_id);

    // Response
    void doneSearching(QString keyword, QVector<Action> actions, int recent_count);
};

#endif // PALETTE_FILTER_H
