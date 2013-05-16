#include "gmail.h"

GMail::GMail(QString login, QString password, QObject *parent):
    QObject(parent), login(login), password(password)
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
                                  + login + QString(":") + password +
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

    vmime::charset ch(vmime::charsets::UTF_8);

    for (std::vector<vmime::ref<message> >::iterator it = msgs.begin();
         it != msgs.end(); ++it) {
        // not seen
        if (((*it)->getFlags() & message::FLAG_SEEN)) {
            //            vmime::ref <vmime::message> parsedMsg = (*it)->getParsedMessage();
             vmime::ref< vmime::message > msg = (*it)->getParsedMessage();
            vmime::messageParser mp(msg);


            vmime::datetime date = mp.getDate();

            string line;
            ostringstream os;

            os << "Date: " << date.getDay() << "." << date.getMonth() <<"." << date.getYear() << " "
                << date.getHour() << ":" << date.getMinute() << ":" << date.getSecond()
                << " " << date.getZone() << endl;

            out << os.str();

            out << "From: " << mp.getExpeditor().getName().getConvertedText(ch) << " " << mp.getExpeditor().getEmail() << "\n";

//            out << "To: " << mp.getRecipients().get
            out << "Subject: " << mp.getSubject().getConvertedText(ch) << "\n";

            std::cout << "Message has " << mp.getAttachmentCount() << " attachment(s)" << std::endl;

            for (int i = 0; i < mp.getAttachmentCount(); i++) {
                vmime::ref<const vmime::attachment> att = mp.getAttachmentAt(i);
                std::cout << "  --" << att->getType().generate() << std::endl;

//                std::cout << "Message has " << mp.getTextPartCount()
//                          << " text part(s)" << std::endl;
            }
            for (int i = 0; i < mp.getTextPartCount(); i++) {
                vmime::ref<const vmime::textPart> tp = mp.getTextPartAt(i);


//                vmime::charset ch(vmime::charsets::UTF_8);

                //text/html
                if (tp->getType().getSubType() == vmime::mediaTypes::TEXT_HTML) {
                    vmime::ref<const vmime::htmlTextPart> htp =
                            tp.dynamicCast<const vmime::htmlTextPart>();

                    htp->getPlainText()->extract(out);
                }
                else {
                    tp->getText()->extract(out);
                }
            }
            out << "\n";
        }
    }
}
    
    
    void myCertVerifier::verify(vmime::ref<vmime::security::cert::certificateChain> chain)
    {
        
        // Obtain the subject's certificate
        vmime::ref <vmime::security::cert::certificate> cert = chain->getAt(0);
        
        std::cout << std::endl;
        std::cout << "Server sent a '" << cert->getType() << "'"
                  << " certificate." << std::endl;
        std::cout << "Do you want to accept this certificate? (Y/n) ";
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
    
