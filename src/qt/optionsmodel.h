// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <QAbstractListModel>

QT_BEGIN_NAMESPACE
class QNetworkProxy;
QT_END_NAMESPACE

/** Interface from Qt to configuration data structure for Bitcoin client.
   To Qt, the options are presented as a list with the different options
   laid out vertically.
   This can be changed to a tree once the settings become sufficiently
   complex.
 */
class OptionsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OptionsModel(QObject *parent = 0);

    enum OptionID {
        StartAtStartup,         // bool
        MinimizeToTray,         // bool
        MapPortUPnP,            // bool
        MinimizeOnClose,        // bool
        ProxyUse,               // bool
        ProxyIP,                // QString
        ProxyPort,              // int
        ProxySocksVersion,      // int
        Fee,                    // qint64
        DisplayUnit,            // BitcoinUnits::Unit
        DisplayAddresses,       // bool
        ThirdPartyTxUrls,       // QString
        Language,               // QString
        CoinControlFeatures,    // bool
        ThreadsScriptVerif,     // int
        DatabaseCache,          // int
        SpendZeroConfChange,    // bool
        
        /////////////////
        // Feature 1   //
        /////////////////

        /* backup wallet options */
        backupOnDemandOpt,      // bool
        backupOnStartOpt,       // bool
        backupOnCloseOpt,       // bool
        backupOnDemandFreqOpt,  // int
        backupFileLocation,     // QString

        /////////////////

        OptionIDRowCount,
    };

    void Init();
    void Reset();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    /* Explicit getters */
    bool getMinimizeToTray() { return fMinimizeToTray; }
    bool getMinimizeOnClose() { return fMinimizeOnClose; }
    int getDisplayUnit() { return nDisplayUnit; }
    bool getDisplayAddresses() { return bDisplayAddresses; }
    QString getThirdPartyTxUrls() { return strThirdPartyTxUrls; }
    bool getProxySettings(QNetworkProxy& proxy) const;
    bool getCoinControlFeatures() { return fCoinControlFeatures; }
    const QString& getOverriddenByCommandLine() { return strOverriddenByCommandLine; }

    /////////////////
    // Feature 1   //
    /////////////////
    
    /* explicit backupOnDemandOpt getter */
    bool getBackupOnDemandOpt() { return fBackupOnDemandOpt; }
    /* explicit backupOnStartOpt getter */
    bool getBackupOnStartOpt() { return fBackupOnStartOpt; }
    /* explicit backupOnCloseOpt getter */
    bool getBackupOnCloseOpt() { return fBackupOnCloseOpt; }
    /* explicit backupOnDemandFreqOpt getter */
    int getBackupOnDemandFreqOpt() { return fBackupOnDemandFreqOpt; }
    /* explicit backupFileLocation getter */
    QString getBackupFileLocation() { return strBackupFileLocation; }
    /////////////////

    /* Restart flag helper */
    void setRestartRequired(bool fRequired);
    bool isRestartRequired();

private:
    /* Qt-only settings */
    bool fMinimizeToTray;
    bool fMinimizeOnClose;
    QString language;
    int nDisplayUnit;
    bool bDisplayAddresses;
    QString strThirdPartyTxUrls;
    bool fCoinControlFeatures;
    /* settings that were overriden by command-line */
    QString strOverriddenByCommandLine;

    /////////////////
    // Feature 1   //
    /////////////////

    /*  private variable for backupOnDemandOpt */
    bool fBackupOnDemandOpt;
    /*  private variable for backupOnStartOpt */
    bool fBackupOnStartOpt;
    /*  private variable for backupOnCloseOpt */
    bool fBackupOnCloseOpt;
    /*  private variable for backupOnDemandFreqOpt */
    int fBackupOnDemandFreqOpt;
    /*  private variable for backupFileLocation */
    QString strBackupFileLocation;
    /////////////////

    /// Add option to list of GUI options overridden through command line/config file
    void addOverriddenOption(const std::string &option);

signals:
    void displayUnitChanged(int unit);
    void transactionFeeChanged(qint64);
    void coinControlFeaturesChanged(bool);
};

#endif // OPTIONSMODEL_H
