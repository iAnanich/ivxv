..  IVXV seadistuste koostamise juhend

.. _kt-krypto:

Krüptovõtmed
------------

Kogumisteenuse andmevahetuse turvamiseks on tarvis luua komplekt
krüptograafilisi võtmeid. Komplekti koosseis sõltub kogumisteenuse tehnilistest
seadistustest.

#. Teenuse krüptovõti ja TLS-sertifikaat - kasutatakse teenuste omavahelise
   suhtluse turvamiseks kõigi teenuste puhul peale vahendusteenuse;

#. Hääletamisteenuse ajatemplipäringute signeerimisvõti - kasutatakse
   ajatemplipäringute signeerimiseks juhul, kui ajatempliteenus on
   registreerimisteenuseks;

#. Mobiil-ID tugiteenuse jagatud krüptimissaladus – kasutatakse sümmeetrilise
   AES-256 krüptimise jaoks. Krüptimissaladusega krüptib Mobiil-ID tugiteenus
   hääletajale väljastatava identsustõendi, mille abil hääletaja enda
   identiteeti teistele teenustele tõendab.



Teenuse krüptovõtme ja TLS-sertifikaadi genereerimine
*****************************************************

Teenuse krüptovõti ja TLS-sertifikaat genereeritakse kõigile teenustele peale
vahendusteenuse. Kõikide teenuste sertifikaadid peavad olema välja antud sama
sertifitseerimiskeskuse (CA – *Certificate Authority*) poolt.


CA sertifikaadi genereerimine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sertifitseerimiskeskuse krüptovõtme ja sertifikaadi genereerimine toimub
järgneva käsuga:

.. code-block:: shell-session

   $ openssl req -newkey rsa:2048 -x509 -nodes -days 365 -out ca.pem -keyout ca.key
     -utf8 -subj "/C=EE/O=Example/OU=IVXV Test Certificates/CN=Service CA"

Käsu väljundiks on failid :file:`ca.key` (võti) ja :file:`ca.pem` (sertifikaat).


Teenuse isendi krüptovõtme ja TLS-sertifikaadi genereerimine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Teenuse isendi krüptovõtme ja sertifikaadipäringu genereerimine toimub järgneva
käsuga:

.. code-block:: shell-session

   $ openssl req -newkey rsa:2048 -nodes -keyout <teenuse-id>-tls.key
     -out etcd-<teenuse-id>-tls.csr -utf8
     -subj "/C=EE/O=Example/OU=IVXV Test Certificates/CN=<teenuse-id>"

Käsu väljundiks on failid :file:`<teenuse-id>-tls.key` (võti) ja
:file:`<teenuse-id>-tls.csr` (sertifikaadipäring).

.. attention::

   Talletusteenuse puhul peab sertifikaadipäringus olema CN väärtuseks teenuse
   identifikaatori asemel hostinimi: erinevalt teistest teenustest ei kasutata
   talletusteenuse puhul alternatiivset TLS nime.

Talletusteenuse isendi TLS-sertifikaadi genereerimine toimub järgneva käsuga:

.. code-block:: shell-session

   $ openssl x509 -req -days 365 -CA ca.pem -CAkey ca.key -set_serial 1
     -extfile service-cert-openssl.conf -extensions ext_<teenuse-tüüp>
     -in <teenuse-id>-tls.csr -out <teenuse-id>-tls.pem

Käsu väljundiks on fail :file:`<teenuse-id>-tls.pem`.

Sertifikaadi genereerimiseks peab failisüsteemis olema seadistusfail
:file:`service-cert-openssl.cnf`.

.. literalinclude:: ../../../tests/testdata/testca/service/service-cert-openssl.cnf
   :caption: Fail service-cert-openssl.cnf
   :language: ini


Hääletamisteenuse ajatemplipäringute signeerimisvõtme ja sertifikaadi genereerimine
***********************************************************************************

Hääletusteenuse registreerimisvõti genereeritakse järgneva käsuga:

.. code-block:: shell-session

   $ openssl genrsa -out tspreg.key 2048

Käsu väljundiks on fail :file:`tspreg.key`.

Hääletusteenuse registreerimisvõtme sertifikaat genereeritakse järgneva käsuga:

.. code-block:: shell-session

   $ openssl req -new -x509 -nodes -days 365 -out tspreg.pem -key tspreg.key
     -utf8 -subj "/C=EE/O=Example/OU=IVXV Test Certificates/CN=Collector Registration"

Käsu väljundiks on fail :file:`tspreg.pem`.

.. note::

   Hääletamisteenuse ajatemplipäringute signeerimisvõti on vaja genereerida
   vaid juhul, kui ajatempliteenust kasutatakse on registreerimisteenuseks.


Mobiil-ID tugiteenusele jagatud krüptimissaladuse genereerimine
***************************************************************

Jagatud krüptimissaladus genereeritakse järgneva käsuga:

.. code-block:: shell-session

   $ openssl rand -out mobid-shared-secret.key 32

Käsu väljundiks on 32 baidi suurune fail :file:`mobid-shared-secret.key`, mida
mobiil-ID teenus hakkab kasutama sümmeetrilise AES-256 krüptimise jaoks.

.. note::

   Mobiil-ID tugiteenuse jagatud krüptimissaladus on vaja genereerida vaid
   juhul, kui Mobiil-ID tugiteenus on kasutusel.

.. vim: sts=3 sw=3 et syntax=rst:
