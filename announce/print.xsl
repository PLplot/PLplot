<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version='1.0'
                xmlns="http://www.w3.org/TR/xhtml1/transitional"
                exclude-result-prefixes="#default">

<xsl:import
  href="/usr/share/sgml/docbook/stylesheet/xsl/nwalsh/fo/docbook.xsl"/>
	
<xsl:template name="footer.table">
  <xsl:param name="pageclass" select="''"/>
  <xsl:param name="sequence" select="''"/>
  <xsl:param name="gentext-key" select="''"/>

  <fo:table table-layout="fixed" width="100%">
    <fo:table-column column-number="1" column-width="100%"/>
    <fo:table-body>
      <fo:table-row height="14pt">
        <fo:table-cell text-align="center">
           <fo:block white-space-collapse="false">
             <fo:page-number/>
	   </fo:block>
        </fo:table-cell>
      </fo:table-row>
    </fo:table-body>
  </fo:table>
</xsl:template>

<xsl:template name="header.table">
</xsl:template>

</xsl:stylesheet>
