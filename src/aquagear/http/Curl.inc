<?php
namespace aquagear\http;

/**
 * Curlクラス.
 * 
 * @package aquagear
 * @subpackage http
 * 
 * @author motonari
 * @version $Id: Curl.inc 535 2011-07-26 14:02:27Z motonari $
 *
 */ 
class Curl extends CoreHttpRequest {
	/**
	 * cURLハンドラ.
	 * 
	 * @var resource
	 */
	private $_curl_handler = null;
	
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	public function __construct($url = null) {		
		$this->_curl_handler = curl_init($url);
		\aquagear\log\Logger::getLogInstance()->info('curl init url [url='.$url.'].');
		if($this->_curl_handler) {
			$this->setOption(CURLOPT_RETURNTRANSFER, true);
		}
	}
	
	/**
	 * オプションの設定.
	 * 
	 * @param string $key curlのオプション値に準ずる
	 * @param mixed $value オプションに応じた値
	 * @return bool
	 * 
	 * @see http://www.php.net/manual/ja/function.curl-setopt.php
	 */
	public function setOption($key, $value) {
		\aquagear\log\Logger::getLogInstance()->debug('curl set opt [key='.$key.'][value='.$value.'].');
		
		return curl_setopt($this->_curl_handler, $key, $value);
	}
	
	/**
	 * curlの実行.
	 * 
	 * @return string
	 */
	public function execute() {
		\aquagear\log\Logger::getLogInstance()->info('curl execute.');
		
		if(! $this->_curl_handler) {
			return false;
		}
				
		$content = curl_exec($this->_curl_handler);
		$this->close();
		
		return $content;
	}

	/**
	 * エラー番号の取得.
	 * 
	 * @return int
	 */
	public function getErrno() {
		
		if($this->_curl_handler) {
			$errno = curl_errno($this->_curl_handler);
			\aquagear\log\Logger::getLogInstance()->debug('curl error no = ' . $errno);
			return $errno;
		}
		
		return false;
	}
	
	/**
	 * エラー文字列の取得.
	 * 
	 * @return string
	 */
	public function getError() {
		if($this->_curl_handler) {
			$err = curl_error($this->_curl_handler);
			\aquagear\log\Logger::getLogInstance()->debug('curl error string = ' . $err);
			return $err;
		}
		
		return false;
	}

	/**
	 * curlの情報の取得.
	 * 
	 * @param srgin $key キー
	 * @param int $opt 取得するオプション.
	 * @return string
	 */
	public function getCurlInfo($opt=CURLINFO_FILETIME) {
		if($this->_curl_handler) {
			$result =  curl_getinfo($this->_curl_handler, $opt);
			\aquagear\log\Logger::getLogInstance()->info('curl info ['. $opt . '=' . $result . ']');
			return $result;
		}
		
		return false;
	}
	
	/**
	 * リソースハンドラの取得.
	 * 
	 * @return resource
	 */
	public function getHandler() {
		return $this->_curl_handler;
	}
	
	/**
	 * ハンドラをクローズする.
	 * 
	 * @return void
	 */
	public function close() {
		if($this->_curl_handler) {
			\aquagear\log\Logger::getLogInstance()->info('curl close.');
			curl_close($this->_curl_handler);
		}
	}
	
	/**
	 * デストラクタ.
	 * 
	 * @return unknown_type
	 */
	public function __destruct() {
		$this->close();
	}
}
?>