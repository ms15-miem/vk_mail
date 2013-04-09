#include "gmail.h"

GMail::GMail(QString login, QString password, QObject *parent):
    login(login), password(password), QObject(parent)
{
#ifdef Q_OS_UNIX
    vmime::platform::setHandler <vmime::platforms::posix::posixHandler>();
#endif
#ifdef Q_OS_WIN32
    vmime::platform::setHandler <vmime::windows::windowsHandler>();
#endif
}

void GMail::connect()
{
    // Connect to the IMAP store
    vmime::ref <vmime::net::session> sess = vmime::create <vmime::net::session>();

    vmime::utility::url storeURL((QString("imaps://")
                                 + login + QString(":") +
                                 QString("@imap.gmail.com:993")).toStdString().c_str());

    vmime::ref <vmime::net::store> store = sess->getStore(storeURL);
    store->setCertificateVerifier(vmime::create<myCertVerifier>());

    store->connect();

    readEmails(store);
}

void GMail::readEmails(vmime::ref <vmime::net::store> store)
{
    // Open the INBOX
    vmime::ref <vmime::net::folder> folder = store->getDefaultFolder();
    folder->open(vmime::net::folder::MODE_READ_WRITE);


    std::vector<vmime::ref<message> > msgs = folder->getMessages();
    folder->fetchMessages(msgs, folder::FETCH_FLAGS);

    vmime::utility::outputStreamAdapter out(std::cout);

    for (std::vector<vmime::ref<message> >::iterator it = msgs.begin();
         it != msgs.end(); ++it) {
        // not seen
        if (((*it)->getFlags() & message::FLAG_SEEN)) {
            vmime::ref <vmime::message> parsedMsg = (*it)->getParsedMessage();
            //            std::cout << "message: ";
            //            parsedMsg->getBody()->generate(out);
            //            cout << endl << endl;


            vmime::messageParser mp(parsedMsg);


            std::cout << "Message has " << mp.getAttachmentCount()
                      << "attachment(s)" << std::endl;

            for (int i = 0; i < mp.getAttachmentCount(); ++i) {
                vmime::ref <const vmime::attachment> att = mp.getAttachmentAt(i);
                std::cout << "  -" << att->getType().generate() << std::endl;
            }

            // Output information about text parts
            std::cout << "Message has " << mp.getTextPartCount()
                      << " text part(s)" << std::endl;

            vmime::charset ch(vmime::charsets::UTF_8);


            for (int i = 0; i < mp.getTextPartCount(); ++i) {
                vmime::ref <const vmime::textPart> tp = mp.getTextPartAt(i);


//                tp->getText()->generate(cout, tp->getCharset());

//                std::cout << tp->getText() << endl;
            }

            std::cout << endl;
        }
    }
}

