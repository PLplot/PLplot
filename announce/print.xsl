<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version='1.0'
                xmlns="http://www.w3.org/TR/xhtml1/transitional"
                exclude-result-prefixes="#default">

<xsl:import
  href="/usr/share/sgml/docbook/stylesheet/xsl/nwalsh/fo/docbook.xsl"/>

<xsl:template name="header.table">
  <xsl:param name="pageclass" select="''"/>
  <xsl:param name="sequence" select="''"/>
  <xsl:param name="gentext-key" select="''"/>

  <!-- default is a single table style for all headers -->
  <!-- Customize it for different page classes or sequence location -->

  <xsl:variable name="candidate">
    <fo:table table-layout="fixed" width="100%">
      <xsl:call-template name="head.sep.rule"/>
      <xsl:choose>
        <xsl:when test="$sequence='odd'">
          <fo:table-column column-number="1" column-width="80%"/>
          <fo:table-column column-number="2" column-width="20%"/>
        </xsl:when>
        <xsl:when test="$sequence='even'">
          <fo:table-column column-number="1" column-width="20%"/>
          <fo:table-column column-number="2" column-width="80%"/>
        </xsl:when>
      </xsl:choose>
      <fo:table-body>
        <fo:table-row height="14pt">
          <fo:table-cell text-align="left">
            <fo:block text-align="start">
              <xsl:choose>
                <xsl:when test="$sequence='even'">
                  <fo:page-number/>
                </xsl:when>
                <xsl:when test="$sequence='odd'">
                  <fo:retrieve-marker retrieve-class-name="section.head.marker"
                    retrieve-position="first-including-carryover"
                    retrieve-boundary="page-sequence"/>
                </xsl:when>
              </xsl:choose>
            </fo:block>
          </fo:table-cell>
          <fo:table-cell text-align="right">
            <fo:block text-align="end">
              <xsl:choose>
                <xsl:when test="$sequence='odd'">
                  <fo:page-number/>
                </xsl:when>
                <xsl:when test="$sequence='even'">
                  <fo:retrieve-marker retrieve-class-name="section.head.marker"
                    retrieve-position="first-including-carryover"
                    retrieve-boundary="page-sequence"/>
                </xsl:when>
              </xsl:choose>
            </fo:block>
          </fo:table-cell>
        </fo:table-row>
      </fo:table-body>
    </fo:table>
  </xsl:variable>

  <!-- Really output a header? -->
  <xsl:choose>
    <xsl:when test="$pageclass = 'titlepage' and $gentext-key = 'book'
                    and $sequence='first'">
      <!-- no, book titlepages have no headers at all -->
    </xsl:when>
    <xsl:when test="$sequence = 'blank' and $headers.on.blank.pages = 0">
      <!-- no output -->
    </xsl:when>
    <xsl:otherwise>
      <xsl:copy-of select="$candidate"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="footer.table">
</xsl:template>

</xsl:stylesheet>
