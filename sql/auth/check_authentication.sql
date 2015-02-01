SELECT
   id,
   role
FROM
   authentication
WHERE
             id = :id
   and password = :password