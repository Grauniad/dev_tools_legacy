#!/usr/bin/env python

import smtplib

import email.mime.text
import email.mime.multipart
import email.mime.image

import copy

#
# struct with TO / FROM etc...
#
class Email_RoutingInfo(object):
    def __init__(self,from_addr="",to=[""], cc=[""], bcc=[""]):
       # Store basic info about destination
       self.TO  = to
       self.CC  = cc
       self.BCC = bcc
       self.FROM = from_addr
       # SMTP won't route to here, but the client will display it...
       self.FAKE_TO = ""

    def ToString(self):
        s = ""
        for a in self.TO:
            s+=a + ";"
        for a in self.FAKE_TO:
            s+=a + ";"

        # Remove trailing ;...
        return s[:-1]

    def CCString(self):
       s = ""
       for a in self.CC:
           s+=a + ";"
       # Remove trailing ;...
       return s[:-1]

    def BCCString(self):
       s = ""
       for a in self.BCC:
           s+=a + ";"
       # Remove trailing ;...
       return s[:-1]

class Local_Router(object):
    def __init__(self):
       self.connection = smtplib.SMTP("localhost")
    def Send(self,msg,routing):
       # Create a local copy to manipulate
       m = copy.deepcopy(msg)
       
       # Now add routing info for the client...
       m['TO'] = routing.ToString() 
       m['CC'] = routing.CCString()
       m['BCC'] = routing.BCCString()

       # As far as SMTP is concerned, everyone is a recipient...
       toAddrs = routing.TO + routing.CC + routing.BCC

       # Do the send...
       self.connection.sendmail(routing.FROM, toAddrs ,m.as_string())

       print toAddrs, m.as_string()


class Email(object):
   def __init__(self,msg):
      #
      # Takes some kind of message created by email.mime
      # 
      self.msg =msg
   def __setitem__(self,name,value):
      self.msg[name] = value

   def as_string(self):
      return self.msg.as_string()

class HtmlMsg(Email):
   def __init__(self, html):
      #
      # Generate an html message
      #
      Email.__init__(self,email.mime.text.MIMEText(html,"html"))

class MultiPart(Email):
    def __init__(self):
        Email.__init__(self,email.mime.multipart.MIMEMultipart("related"))
        self.msg.preamble = "This is a multi-part message in MIME format"
    def AttachMsg(self,msg):
        self.msg.attach(msg.msg)
    def AttachImage(self,partId,fname):
        fp = open(fname)
        image = email.mime.image.MIMEImage(fp.read())
        fp.close()
        image.add_header("Content-ID","<" + partId + ">")
        self.msg.attach(image)


