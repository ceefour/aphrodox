using System;

namespace NDoc.Documenter.LinearHtml
{
	/// <summary>
	/// Used as an extension object to the xslt processor to allow
	/// retrieving user-provided raw html.
	/// </summary>
	public class LhExternalHtmlProvider
	{
		/// <summary>
		/// Contructor.
		/// </summary>
		/// <param name="config">LinearHtmlDocumenterConfig from which the property values can be retrieved.</param>
		/// <param name="fileName">Name of the HTML file that is currently being generated by the xslt processor.</param>
		public LhExternalHtmlProvider(LinearHtmlDocumenterConfig config, string fileName)
		{
			_config = config;
			_fileName = fileName;
		}

		/// <summary>
		/// Retrieves user-provided raw html to use as page headers.
		/// </summary>
		/// <param name="topicTitle">The title of the current topic.</param>
		/// <returns></returns>
		public string GetHeaderHtml(string topicTitle)
		{
			string headerHtml = _config.HeaderHtml;

			if (headerHtml == null)
				return string.Empty;

			headerHtml = headerHtml.Replace("%TOPIC_TITLE%", topicTitle);
			headerHtml = headerHtml.Replace("%FILE_NAME%", _fileName);

			return headerHtml;
		}

		/// <summary>
		/// Retrieves user-provided raw html to use as page footers.
		/// </summary>
		/// <param name="assemblyName">The name of the assembly for the current topic.</param>
		/// <param name="assemblyVersion">The version of the assembly for the current topic.</param>
		/// <param name="topicTitle">The title of the current topic.</param>
		/// <returns></returns>
		public string GetFooterHtml(string assemblyName, string assemblyVersion, string topicTitle)
		{
			string footerHtml = _config.FooterHtml;

			if (footerHtml == null)
				return string.Empty;

			footerHtml = footerHtml.Replace("%ASSEMBLY_NAME%", assemblyName);
			footerHtml = footerHtml.Replace("%ASSEMBLY_VERSION%", assemblyVersion);
			footerHtml = footerHtml.Replace("%TOPIC_TITLE%", topicTitle);
			footerHtml = footerHtml.Replace("%FILE_NAME%", _fileName);

			return footerHtml;
		}

		private LinearHtmlDocumenterConfig _config;
		private string _fileName;
	}
}
