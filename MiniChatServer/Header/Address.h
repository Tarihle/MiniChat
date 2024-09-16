#pragma once

//struct Addrinfo
//{
//    int                 ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
//    int                 ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
//    int                 ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
//    int                 ai_protocol;  // use 0 for "any"
//    size_t              ai_addrlen;   // size of ai_addr in bytes
//    struct SockAddr*    ai_addr;      // struct sockaddr_in or _in6
//    char*               ai_canonname; // full canonical hostname
//    struct Addrinfo*    ai_next;      // linked list, next node
//};
//
//struct SockAddr
//{
//    unsigned short    sa_family;    // address family, AF_xxx
//    char              sa_data[14];  // 14 bytes of protocol address
//};