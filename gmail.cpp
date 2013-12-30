#include "gmail.h"

GMail::GMail(int checkIntervalMsec, QObject *parent):
    QObject(parent), SettingsManager("gmail"), checkIntervalMsec(checkIntervalMsec)
{
#ifdef Q_OS_UNIX
    vmime::platform::setHandler <vmime::platforms::posix::posixHandler>();
#endif
#ifdef Q_OS_WIN32
    vmime::platform::setHandler <vmime::windows::windowsHandler>();
#endif

    loadAuthData();

    checkEmailTimer = new QTimer(this);

    QObject::connect(checkEmailTimer, SIGNAL(timeout()), SLOT(readEmails()));
}

void GMail::connect()
{
    // Connect to the IMAP store
    vmime::ref <vmime::net::session> sess = vmime::create <vmime::net::session>();
    
    vmime::utility::url storeURL((QString("imaps://")
                                  + login + QString(":") + password +
                                  QString("@imap.gmail.com:993")).toStdString().c_str());
    
    store = sess->getStore(storeURL);
    store->setCertificateVerifier(vmime::create<myCertVerifier>());
    
    store->connect();
}

void GMail::setCheckInterval(int msec)
{
    checkIntervalMsec = msec;
}

int GMail::getCheckInterval()
{
    return checkIntervalMsec;
}

void GMail::readEmails()
{
    // Open the INBOX
    vmime::ref <vmime::net::folder> folder = store->getDefaultFolder();
    folder->open(vmime::net::folder::MODE_READ_WRITE);
    
    
    std::vector<vmime::ref<message> > msgs = folder->getMessages();
    folder->fetchMessages(msgs, folder::FETCH_FLAGS);

    vmime::charset ch(vmime::charsets::UTF_8);

    for (std::vector<vmime::ref<message> >::iterator it = msgs.begin();
         it != msgs.end(); ++it) {

#ifdef QT_DEBUG
        // seen
        if (((*it)->getFlags() & message::FLAG_SEEN)) {
#else
        // not seen
        if (!((*it)->getFlags() & message::FLAG_SEEN)) {
#endif


            ostringstream outString;
            vmime::utility::outputStreamAdapter out(outString);

            vmime::ref< vmime::message > msg = (*it)->getParsedMessage();

            vmime::messageParser mp(msg);

            vmime::datetime date = mp.getDate();


            // hz pochemu i = 1 (inache soobshenie vyvoditsya 2 raza
            for (int i = 1; i < mp.getTextPartCount(); i++) {
                vmime::ref<const vmime::textPart> tp = mp.getTextPartAt(i);

                //text/html
                if (tp->getType().getSubType() == vmime::mediaTypes::TEXT_HTML) {
                    vmime::ref<const vmime::htmlTextPart> htp =
                            tp.dynamicCast<const vmime::htmlTextPart>();

                    htp->getPlainText()->extract(out);
                }
                else {
                    tp->getText()->extract(out);
                }
                out << "\n";
            }

            QDateTime dateTime(
                        QDate(date.getYear(), date.getMonth(), date.getDay()),
                        QTime(date.getHour(), date.getMinute(), date.getSecond())
                        );

            QString from = (mp.getExpeditor().getName().getConvertedText(ch)
                            + string(" ") + mp.getExpeditor().getEmail()).c_str();


            QString text = outString.str().c_str();

            QString subject = mp.getSubject().getConvertedText(ch).c_str();

            emit unreadedMessage(
                        new GMailMessage(
                            dateTime,
                            from,
                            subject,
                            text
                            )
                        );


#ifdef QT_NO_DEBUG
            QEventLoop loop;
            QTimer sleepTimer;
            QObject::connect(&sleepTimer, SIGNAL(timeout()), &loop, SLOT(quit()));

            sleepTimer.start(5000);
            loop.exec();
#endif

            //            {
            //                ostringstream os;
            //                os << "Date: " << date.getDay() << "." << date.getMonth() <<"."
            //                   << date.getYear() << " "
            //                   << date.getHour() << ":" << date.getMinute() << ":" << date.getSecond()
            //                   << " " << date.getZone() << endl;

            //                out << os.str();
            //            }

            //            out << "From: " << mp.getExpeditor().getName().getConvertedText(ch)
            //                << " " << mp.getExpeditor().getEmail() << "\n";

            //            //            out << "To: " << mp.getRecipients().get
            //            out << "Subject: " << mp.getSubject().getConvertedText(ch) << "\n";

            //            {
            //                ostringstream os;
            //                os << "Message has " << mp.getAttachmentCount()
            //                   << " attachment(s)" << "\n";
            //                out << os.str();
            //            }

            //            for (int i = 0; i < mp.getAttachmentCount(); i++) {
            //                vmime::ref<const vmime::attachment> att = mp.getAttachmentAt(i);
            //                out << "  --" << att->getType().generate() << "\n";

            //            }

            //            out << "\n";
        }
    }
}

void GMail::startCheckCycle()
{
    // zachem-to nado vernutysya v QCoreApplication:exec() inache segfault
    QTimer::singleShot(0, this, SLOT(readEmails()));
#ifdef QT_NO_DEBUG
    checkEmailTimer->start(checkIntervalMsec);
#endif
}

void myCertVerifier::verify(vmime::ref<vmime::security::cert::certificateChain> chain)
{

    // Obtain the subject's certificate
    vmime::ref <vmime::security::cert::certificate> cert = chain->getAt(0);

    std::cout << std::endl;
    std::cout << "Server sent a '" << cert->getType() << "'"
              << " certificate." << std::endl;
    std::cout << "Do you want to accept this certificate? (Y/n) " << std::endl;
    std::cout.flush();

    std::string answer = "Y";
    //    std::getline(std::cin, answer);

    if (answer.length() != 0 && (answer[0] == 'Y' || answer[0] == 'y'))
        return; // OK, we trush the certificate

    // Don't trust this certificate
    vmime::exceptions::certificate_verification_exception e
            ("Don't trust this certificate");
    throw e;

}


void GMail::saveAuthData() const
{
}

void GMail::loadAuthData()
{
    login = cfg->value("login").toString();
    password = cfg->value("password").toString();
    if (login.isEmpty() || password.isEmpty()) {
        QTextStream(stderr) << "Put a login and a password of a gmail account to the config";
        if(login.isEmpty()) cfg->setValue("login", "");
        if(password.isEmpty()) cfg->setValue("password", "");
    }
}
