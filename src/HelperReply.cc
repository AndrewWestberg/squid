/*
 * DEBUG: section 84    Helper process maintenance
 * AUTHOR: Amos Jeffries
 */
#include "squid.h"
#include "ConfigParser.h"
#include "HelperReply.h"
#include "helper.h"
#include "rfc1738.h"
#include "SquidString.h"
#include "Debug.h"

HelperReply::HelperReply(char *buf, size_t len) :
        result(HelperReply::Unknown),
        whichServer(NULL)
{
    parse(buf,len);
}

void
HelperReply::parse(char *buf, size_t len)
{
    debugs(84, 3, "Parsing helper buffer");
    // check we have something to parse
    if (!buf || len < 1) {
        // for now ensure that legacy handlers are not presented with NULL strings.
        debugs(84, 3, "Reply length is smaller than 1 or none at all ");
        other_.init(1,1);
        other_.terminate();
        return;
    }

    char *p = buf;

    // optimization: do not consider parsing result code if the response is short.
    // URL-rewriter may return relative URLs or empty response for a large portion
    // of its replies.
    if (len >= 2) {
        debugs(84, 3, "Buff length is larger than 2");
        // some helper formats (digest auth, URL-rewriter) just send a data string
        // we must also check for the ' ' character after the response token (if anything)
        if (!strncmp(p,"OK",2) && (len == 2 || p[2] == ' ')) {
            debugs(84, 3, "helper Result = OK");
            result = HelperReply::Okay;
            p+=2;
        } else if (!strncmp(p,"ERR",3) && (len == 3 || p[3] == ' ')) {
            debugs(84, 3, "helper Result = ERR");
            result = HelperReply::Error;
            p+=3;
        } else if (!strncmp(p,"BH",2) && (len == 2 || p[2] == ' ')) {
            debugs(84, 3, "helper Result = BH");
            result = HelperReply::BrokenHelper;
            p+=2;
        } else if (!strncmp(p,"TT ",3)) {
            // NTLM challenge token
            result = HelperReply::TT;
            p+=3;
            // followed by an auth token
            char *w1 = strwordtok(NULL, &p);
            if (w1 != NULL) {
                MemBuf authToken;
                authToken.init();
                authToken.append(w1, strlen(w1));
                notes.add("token",authToken.content());
            } else {
                // token field is mandatory on this response code
                result = HelperReply::BrokenHelper;
                notes.add("message","Missing 'token' data");
            }

        } else if (!strncmp(p,"AF ",3)) {
            // NTLM/Negotate OK response
            result = HelperReply::Okay;
            p+=3;
            // followed by:
            //  an optional auth token and user field
            // or, an optional username field
            char *w1 = strwordtok(NULL, &p);
            char *w2 = strwordtok(NULL, &p);
            if (w2 != NULL) {
                // Negotiate "token user"
                MemBuf authToken;
                authToken.init();
                authToken.append(w1, strlen(w1));
                notes.add("token",authToken.content());

                MemBuf user;
                user.init();
                user.append(w2,strlen(w2));
                notes.add("user",user.content());

            } else if (w1 != NULL) {
                // NTLM "user"
                MemBuf user;
                user.init();
                user.append(w1,strlen(w1));
                notes.add("user",user.content());
            }
        } else if (!strncmp(p,"NA ",3)) {
            // NTLM fail-closed ERR response
            result = HelperReply::Error;
            p+=3;
        }

        for (; xisspace(*p); ++p); // skip whitespace
    }

    const mb_size_t blobSize = (buf+len-p);
    other_.init(blobSize+1, blobSize+1);
    other_.append(p, blobSize); // remainders of the line.

    // NULL-terminate so the helper callback handlers do not buffer-overrun
    other_.terminate();

    parseResponseKeys();

    // Hack for backward-compatibility: BH used to be a text message...
    if (other().hasContent() && result == HelperReply::BrokenHelper) {
        notes.add("message",other().content());
        modifiableOther().clean();
    }
}

void
HelperReply::parseResponseKeys()
{
    // parse a "key=value" pair off the 'other()' buffer.
    while (other().hasContent()) {
        char *p = modifiableOther().content();
        while (*p && *p != '=' && *p != ' ') ++p;
        if (*p != '=')
            return; // done. Not a key.

        // whitespace between key and value is prohibited.
        // workaround strwordtok() which skips whitespace prefix.
        if (xisspace(*(p+1)))
            return; // done. Not a key.

        *p = '\0';
        ++p;

        const String key(other().content());

        // the value may be a quoted string or a token
        const bool urlDecode = (*p != '"'); // check before moving p.
        char *v = strwordtok(NULL, &p);
        if (v != NULL && urlDecode && (p-v) > 2) // 1-octet %-escaped requires 3 bytes
            rfc1738_unescape(v);
        const String value(v?v:""); // value can be empty, but must not be NULL

        notes.add(key, value);

        modifiableOther().consume(p - other().content());
        modifiableOther().consumeWhitespacePrefix();
    }
}

std::ostream &
operator <<(std::ostream &os, const HelperReply &r)
{
    os << "{result=";
    switch (r.result) {
    case HelperReply::Okay:
        os << "OK";
        break;
    case HelperReply::Error:
        os << "ERR";
        break;
    case HelperReply::BrokenHelper:
        os << "BH";
        break;
    case HelperReply::TT:
        os << "TT";
        break;
    case HelperReply::Unknown:
        os << "Unknown";
        break;
    }

    // dump the helper key=pair "notes" list
    if (r.notes.notes.size() > 0) {
        os << ", notes={";
        for (Notes::NotesList::const_iterator m = r.notes.notes.begin(); m != r.notes.notes.end(); ++m) {
            for (Note::Values::iterator v = (*m)->values.begin(); v != (*m)->values.end(); ++v) {
                os << ',' << (*m)->key << '=' << ConfigParser::QuoteString((*v)->value);
            }
        }
        os << "}";
    }

    if (r.other().hasContent())
        os << ", other: \"" << r.other().content() << '\"';

    os << '}';

    return os;
}
