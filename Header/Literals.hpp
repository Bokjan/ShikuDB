#ifndef SHIKU_LITERALS_HPP_
#define SHIKU_LITERALS_HPP_
#define _SIZEOF(x) (sizeof(x) - 1)
#ifdef zh_cn
#include "Lang_zh_cn.hpp"
#else
#include "Lang_en_us.hpp"
#endif //Language selection

#define HTTPD_EXTRA_HEADERS "Content-Type: application/json"
#define SHIKUDB_JSON_FIELD_MESSAGE "message"

#endif //SHIKU_LITERALS_HPP_