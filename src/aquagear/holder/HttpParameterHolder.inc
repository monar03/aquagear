<?php
namespace aquagear\holder;

/**
 * HTTP関係のパラメータホルダ.
 * 
 * @package aquagear
 * @subpackage holder
 * 
 * @author motonari
 * @version $Id: HttpParameterHolder.inc 538 2011-07-31 12:52:00Z motonari $
 *
 */ 
class HttpParameterHolder extends CoreHolder {
	/**
	* URI
	*
	* @var string
	*/
	private static $uri = null;
	
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	private function __construct() {
		
	}
	
	/**
	 * Httpインスタンスの取得.
	 * 
	 * @param mixed $property プロパティ
	 * @return object
	 */
	public static function getInstance() {
		static $instance = null;
		
		if($instance === null) {
			$instance = new \aquagear\holder\HttpParameterHolder();
		}
		
		return $instance;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__get()
	 */
	public function __get($arg) {
		
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/\aquagear\Object#__set()
	 */
	public function __set($arg, $value) {
		
	}
	
	/**
	 * GET値の取得.
	 * 
	 * @param string $arg GET添え字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getGetValue($arg, $default=false, $filter=parent::HTML_FILTER) {
		if(isset($_GET[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Get value [arg=$arg][_GET=".$_GET[$arg]."]");
			return parent::filterValue($_GET[$arg], $filter);
		}
		
		\aquagear\log\Logger::getLogInstance()->debug("get Get value [arg=$arg][_GET=]");
		return $default;
	}
	
	/**
	 * POST値の取得.
	 * 
	 * @param string $arg POST値の添え字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getPostValue($arg, $default=false, $filter=parent::HTML_FILTER) {
		if(isset($_POST[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Post value [arg=$arg][_POST=".$_POST[$arg]."]");
			return parent::filterValue($_POST[$arg], $filter);
		}

		\aquagear\log\Logger::getLogInstance()->debug("get Post value [arg=$arg][_POST=]");
		return $default;
	}

	/**
	 * REQUEST値の取得.
	 * 
	 * @param string $arg REQUEST値の添え字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getRequestValue($arg, $default=false, $filter=parent::HTML_FILTER) {
		if(isset($_REQUEST[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Request value [arg=$arg][_REQUEST=".$_REQUEST[$arg]."]");
			return parent::filterValue($_REQUEST[$arg], $filter);
		}

		\aquagear\log\Logger::getLogInstance()->debug("get Request value [arg=$arg][_REQUEST=]");
		return $default;
	}
	
	/**
	 * クッキーの値の取得
	 * 
	 * @param string $arg COOKIE値の添字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getCookieValue($arg, $default=false, $filter=parent::HTML_FILTER) {
		if(isset($_COOKIE[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Cookie value [arg=$arg][_COOKIE=".$_COOKIE[$arg]."]");
			return parent::filterValue($_COOKIE[$arg], $filter);
		}

		\aquagear\log\Logger::getLogInstance()->debug("get Cookie value [arg=$arg][_Cookie=]");	
		return $default;
	}
	
	/**
	 * データのセット.
	 * 
	 * @param string $arg 添え字
	 * @param mixed $value 代入値
	 * @return void
	 */
	public function setCookieValue($arg, $value, $expire=0, $path='', $domain='', $secure=false, $httponly=true) {
		\aquagear\log\Logger::getLogInstance()->debug("set cookie value [arg=$arg][value=$value]");
		setcookie($arg, $value, $expire, $path, $domain, $secure, $httponly);
	}	
	
	/**
	 * File値の取得.
	 * 
	 * @param string $arg File値の添え字
	 * 
	 * @return mixed
	 */
	public function getFileValue($arg) {
		if(isset($_FILES[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Post value [arg=$arg][_POST=".$_FILES[$arg]."]");
			return $_FILES[$arg];
		}

		\aquagear\log\Logger::getLogInstance()->debug("get Post value [arg=$arg][_FILES=]");
		return false;
	}
	
	/**
	 * ENV値の取得.
	 * 
	 * @param string $arg ENV値の添字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getEnvValue($arg, $default=false, $filter=parent::NO_FILTER) {
		if(isset($_ENV[$arg])) {
			\aquagear\log\Logger::getLogInstance()->debug("get Server value [arg=$arg][_ENV=".$_ENV[$arg]."]");
			return parent::filterValue($_ENV[$arg], $filter);
		}
		
		\aquagear\log\Logger::getLogInstance()->debug("get Env value [arg=$arg][_ENV=]");	
		return $default;
	}
	
	/**
	 * SERVER値の取得.
	 * 
	 * @param string $arg SEVER値の添え字
	 * @param int $filter フィルターの変数
	 * 
	 * @return string
	 */
	public function getServerValue($arg, $default=false, $filter=parent::NO_FILTER) {
		if(isset($_SERVER[$arg])) {
			return parent::filterValue($_SERVER[$arg], $filter);
		}
		
		return $default;
	}	
	
	/**
	* URIを取得する.
	*
	* @param bool $getparm ゲットパラメーターをつけるかどうか.
	*
	* @return string
	*
	*/
	public function getURI($getparm=false) {
	
		if(self::$uri === null) {
			$tmp_uri = \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('REQUEST_URI');
	
			$tmp_path = explode('?', $tmp_uri, 2);
			$tmp_path[0] = rtrim($tmp_path[0],'\/');
			if($tmp_path[0] === '') {
				$tmp_path[0] = '/index';
			}
				
			$routing_uri = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . "/routing.ini")->getValue(preg_replace('/\//', '__', $tmp_path[0]));
			if($routing_uri !== false) {
				$tmp_path[0] = preg_replace('/__/', '/', $routing_uri);
			}
				
			self::$uri = implode('?', $tmp_path);
		}
	
		$path = "";
		if($getparm) {
			$path = self::$uri;
		}
		else {
			$tmp_path = explode('?', self::$uri, 2);
			$path = $tmp_path[0];
		}
	
		return $path;
	}
	
	/**
	 * URIをフォワードする.元のURIはパラメータとして、後ろにつけている.
	 *
	 * @param string $uri uri
	 *
	 * @return void
	 *
	 */
	public function rewriteUri($uri) {
		$tmp_path = explode('?', self::$uri, 2);
		$path = $uri . '?' . 'src=' . $tmp_path[0] . '&' . $tmp_path[1];
		self::$uri = $path;
	}
}
?>