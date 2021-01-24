#include "email.h"

//static std::string payloadText[11];

//std::string dateTimeNow();
//std::string generateMessageId();

void setPayloadText(const std::string& to, const std::string& from, const std::string& cc, const std::string& nameFrom, const std::string& subject, const std::string& body)
{
	payloadText[0] = "Date: " + dateTimeNow();
	payloadText[1] = "To: <" + to + ">\r\n";
	payloadText[2] = "From: <" + from + "> (" + nameFrom + ")\r\n";
	payloadText[3] = "Cc: <" + cc + "> (" + nameFrom + ")\r\n";
	payloadText[4] = "Message-ID: <" + generateMessageId() + "@" + from.substr(from.find('@') + 1) + ">\r\n";
	payloadText[5] = "Subject: " + subject + "\r\n";
	payloadText[6] = "\r\n";
	payloadText[7] = body + "\r\n";
	payloadText[8] = "\r\n";
	payloadText[9] = "\r\n"; // "It could be a lot of lines, could be MIME encoded, whatever.\r\n";
	payloadText[10] = "\r\n"; // "Check RFC5322.\r\n";
}

std::string dateTimeNow()
{
	static const char* DAY_NAMES[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	static const char* MONTH_NAMES[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	const int RFC1123_TIME_LEN = 29;
	time_t t;
	struct tm tm;

	std::string ret;
	ret.resize(RFC1123_TIME_LEN);

	time(&t);
	gmtime_s(&tm, &t);

	strftime(&ret[0], RFC1123_TIME_LEN + 1, "---, %d --- %Y %H:%M:%S GMT", &tm);
	memcpy(&ret[0], DAY_NAMES[tm.tm_wday], 3);
	memcpy(&ret[8], MONTH_NAMES[tm.tm_mon], 3);

	return ret;
}

std::string generateMessageId()
{
	const int MESSAGE_ID_LEN = 37;
	time_t t;
	struct tm tm;

	std::string ret;
	ret.resize(15);

	time(&t);
	gmtime_s(&tm, &t);

	strftime(const_cast<char*>(ret.c_str()),
		MESSAGE_ID_LEN,
		"%Y%m%d%H%M%S.",
		&tm);

	ret.reserve(MESSAGE_ID_LEN);

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	while (ret.size() < MESSAGE_ID_LEN) {
		ret += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return ret;
}

//struct upload_status { int lines_read; };

static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp)
{
	std::string s = generateMessageId();

	static const char* pt[12] = {};

	for (int i = 0; i < 11; ++i) {
		pt[i] = payloadText[i].c_str();
	}

	pt[11] = NULL;

	struct upload_status* upload_ctx = (struct upload_status*)userp;
	const char* data;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	data = pt[upload_ctx->lines_read];

	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;

		return len;
	}

	return 0;
}

CURLcode sendEmail(const std::string & to,
	const std::string & from,
	const std::string & cc,
	const std::string & nameFrom,
	const std::string & subject,
	const std::string & body,
	const std::string & url,
	const std::string & password)
{
	CURLcode ret = CURLE_OK;

	struct curl_slist* recipients = NULL;
	struct upload_status upload_ctx;

	upload_ctx.lines_read = 0;

	CURL* curl = curl_easy_init();

	setPayloadText(to, from, cc, nameFrom, subject, body);

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // skip peer verification
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // skip hostname verification

		curl_easy_setopt(curl, CURLOPT_USERNAME, from.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
		//curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, ("<" + from + ">").c_str());
		recipients = curl_slist_append(recipients, ("<" + to + ">").c_str());
		// recipients = curl_slist_append(recipients, ("<" + cc + ">").c_str());

		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		ret = curl_easy_perform(curl);

		if (ret != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
		}

		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}

	return ret;
}
